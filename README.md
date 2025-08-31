
<html lang="en">
<head>
<meta charset="utf-8">

<meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>

<h1>ObradaSignalaProjekat</h1>
<p>A small C++ project (CMake) with simple signal/data processing utilities:
<strong>Run-Length Encoding (RLE)</strong> helpers and basic <strong>tree</strong> structures/“reduce” operations.</p>

<hr>

<h2 id="table-of-contents">Table of contents</h2>
<ol>
  <li><a href="#overview">Overview</a></li>
  <li><a href="#project-layout">Project layout</a></li>
  <li><a href="#requirements">Requirements</a></li>
  <li><a href="#build">Build</a></li>
  <li><a href="#run">Run</a></li>
  <li><a href="#usage">Usage examples</a></li>
  <li><a href="#details">Details (RLE &amp; reduce)</a></li>
  <li><a href="#contributing">Contributing</a></li>
  <li><a href="#license">License</a></li>
</ol>

<hr>

<h2 id="overview">1) Overview</h2>
<ul>
  <li><strong>RLE</strong>: minimal, lossless compression of repeated values (e.g., “AAAABBC” → “A4B2C1”).</li>
  <li><strong>Tree &amp; reduce</strong>: node type and helpers to aggregate/fold values over a tree.</li>
</ul>

<h2 id="project-layout">2) Project layout</h2>
<pre><code>.
├─ CMakeLists.txt
├─ README.md
├─ main.cpp
├─ TreeNode.h
├─ TreeNode.cpp
├─ reduce.h
├─ reduce.cpp
├─ rleReduce.h
└─ rleReduce.cpp
</code></pre>

<h2 id="requirements">3) Requirements</h2>
<ul>
  <li>C++17 compiler (<code>g++</code> or <code>clang++</code>)</li>
  <li>CMake 3.10+</li>
</ul>

<h2 id="build">4) Build</h2>
<pre><code>git clone https://github.com/milutin2002/ObradaSignalaProjekat.git
cd ObradaSignalaProjekat
mkdir build &amp;&amp; cd build
cmake ..
cmake --build . --config Release
</code></pre>

<h2 id="run">5) Run</h2>
<p>From the <code>build</code> directory (the binary name may differ depending on your generator):</p>
<pre><code>./ObradaSignalaProjekat
</code></pre>

<h2 id="usage">6) Usage examples</h2>
<p><em>Note:</em> Adjust to your actual <code>main.cpp</code> interface (menu/flags). If it reads from stdin, you can try:</p>

<h3>6.1 RLE quick test (stdin)</h3>
<pre><code>echo "AAAABBBCCDAA" | ./ObradaSignalaProjekat
</code></pre>
<p>Expected compressed idea: <code>A4B3C2D1A2</code> (exact format depends on implementation).</p>

<h3>6.2 Reduce over a tree (example idea)</h3>
<p>If the program builds a demo tree internally (or from a file) and prints an aggregate (size, depth, sum), you should see a computed result in the console. If it accepts a file, run:</p>
<pre><code>./ObradaSignalaProjekat path/to/input.txt
</code></pre>
<p>Document the exact input format here when finalized.</p>

<h2 id="details">7) Details</h2>

<h3>7.1 Run-Length Encoding (RLE)</h3>
<ul>
  <li><strong>Goal</strong>: Replace runs of identical symbols with (symbol, count).</li>
  <li><strong>Best for</strong>: Data with long runs of the same value.</li>
  <li><strong>Limitations</strong>: Not useful for high-entropy or rapidly changing data.</li>
</ul>

<h3>7.2 Reduce (&ldquo;fold&rdquo;) over trees</h3>
<ul>
  <li><strong>Idea</strong>: Recursively combine child results into a parent result (e.g., sum, max depth).</li>
  <li><strong>Typical uses</strong>: size, height, checksum/aggregate, mapping/transforming node values.</li>
</ul>

<h2 id="contributing">8) Contributing</h2>
<ul>
  <li>Use feature branches and open a Pull Request.</li>
  <li>Keep functions small and documented (brief comment for inputs/outputs).</li>
  <li>Consider adding unit tests (e.g., GoogleTest) for RLE and reduce functions.</li>
</ul>

<h2 id="license">9) License</h2>
<p>No license file currently. If you want others to reuse this code, add a <code>LICENSE</code> (MIT/Apache-2.0, etc.).</p>

<hr>

<p><small>Maintainer: <a href="https://github.com/milutin2002">milutin2002</a></small></p>

</body>
</html>
