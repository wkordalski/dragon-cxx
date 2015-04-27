/*
 * Copyright (c) 2015 Wojciech Kordalski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Imports the tree from persistent representation
 */

#pragma once

#include "../containers.hpp"
#include "../node.hpp"
#include "../visitor.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>

namespace dragon
{
  class Importer : public Visitor
  {
    typedef void (Importer::*decode_func)(Local &);

    boost::archive::binary_iarchive ar;
    std::unordered_map<int, int> readdress;
    std::unordered_set<int> required;
		std::unordered_map<int, std::vector<std::function<void()>>> deferred;

    static decode_func decoder[];

  public:
    Importer(std::istream &out) : ar(out) {}

    LVector deserialize();

  protected:
    Member translate(Handle h)
    {
      if(int(h) == 0) return Handle();
      if(readdress.count(int(h)) == 0)
      {
        Handle nh(nullptr);
        readdress[int(h)] = int(nh);
        required.insert(int(h));
      }
      return Handle(readdress[int(h)]);
    }
    MVector translate(MVector v)
		{
			MVector ret;
			ret.reserve(v.size());
			std::transform(v.begin(), v.end(),
										 std::back_inserter(ret),
										 [this](Handle h){ return translate(h); });
			return ret;
		}
		
		void defer(Handle h, std::function<void()> f)
		{
			if(required.count(int(h)))
			{
				if(deferred.count(int(h)))
					deferred[int(h)].push_back(f);
				else
					deferred[int(h)] = {f};
			}
			else
			{
				f();
			}
		}

  protected:
    template<class T>
    void read(Local &h);
  };
}
