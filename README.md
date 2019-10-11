Software for PN creation based on Regions theory
========================

Part of code is based on PBLib (a fork of MiniSAT 2.2) and NetworkX.

### Key principles:
- Creation of Petri Nets from Transition Systems
- Decomposition of Transition Systems into sets of  interacting State Machines

### Supported extensions

Input extensions: .g

Output extensions: .dot

Building
--------

```bash
make
cd cmake-build-debug
make
```

Execution
---------

### a) TS to PN flow

Quick way (requirese graphviz library)

```Bash
./execute.sh
```

Traditional way

```Bash
./TS_splitter <file_path> S
```
Debug info mode
```Bash
./TS_splitter <file_path> D
```

### b) TS to Synchronized SMs flow

```Bash
./TS_splitter <file_path> M
```

With the creation of a log file:

```Bash
./TS_splitter <file_path> ML
```
Debug info mode:

```Bash
./TS_splitter <file_path> MD
```

Write output SMs in .dot extension:
```Bash
./TS_splitter <file_path> MO
```

PN visualization
----------------

Graphviz library is required

```bash
dot -Tps filename.dot -o outfile.ps
```

## License ##

>BSD 3-Clause License
>
>TStoPN flow -- Copyright (c) 2018, Viktor Teren, Valentina Napoletani
TS decomposition flow -- Copyright (c) 2019, Viktor Teren
All rights reserved.
>
>Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
>
>* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
>
>* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
>
>* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
>
>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.