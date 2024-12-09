Traceback (most recent call last):
  File "/home/rayb/.local/share/pipx/venvs/competitive-verifier/lib/python3.12/site-packages/competitive_verifier/oj_resolve/resolver.py", line 181, in resolve
    bundled_code = language.bundle(path, basedir=basedir)
                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/home/rayb/.local/share/pipx/venvs/competitive-verifier/lib/python3.12/site-packages/competitive_verifier/oj/verify/languages/cplusplus.py", line 252, in bundle
    bundler.update(path)
  File "/home/rayb/.local/share/pipx/venvs/competitive-verifier/lib/python3.12/site-packages/competitive_verifier/oj/verify/languages/cplusplus_bundle.py", line 477, in update
    raise BundleErrorAt(
competitive_verifier.oj.verify.languages.cplusplus_bundle.BundleErrorAt: lib/template.cpp: line 7: unable to process #include in #if / #ifdef / #ifndef other than include guards
