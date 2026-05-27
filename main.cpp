/* MIT License

Copyright (c) 2026 Savva Savenkov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#include "CGlauberModule.hh"
#include "CPreEquilibriumModule.hh"
#include "CRootModule.hh"
#include "Deexcitation/DeexcitationModule.h"

#include <chrono>
#include <iostream>
#include <memory>

using namespace cola;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "Need more parameters to launch\n";
    return 0;
  }
  std::vector<string> params(argv + 1, argv + argc);
  int events = 0;
  try {
    events = std::stoi(params[1]);
  } catch (std::exception e) {
    std::cout << e.what() << '\n';
    return 0;
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  // Create MetaProcessor and register needed filters (dependency injection)
  auto filter_map = CGlauberModule().GetModuleFilters();
  filter_map.merge(CPreEquilibriumModule().GetModuleFilters());
  filter_map.merge(DeexcitationModule().GetModuleFilters());
  filter_map.merge(CRootModule().GetModuleFilters());
  cola::MetaProcessor metaProcessor(std::move(filter_map));

  // Assemble manager and run
  cola::ColaRunManager manager(metaProcessor.Parse(params[0]));
  manager.Run(events);

  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = t2 - t1;
  std::cout << '\n' << duration.count() << "ms\n";
  return 0;
}
