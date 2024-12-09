Traceback (most recent call last):
  File "/home/rayb/.local/share/pipx/venvs/competitive-verifier/lib/python3.12/site-packages/competitive_verifier/oj_resolve/resolver.py", line 181, in resolve
    bundled_code = language.bundle(path, basedir=basedir)
                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/home/rayb/.local/share/pipx/venvs/competitive-verifier/lib/python3.12/site-packages/competitive_verifier/oj/verify/languages/cplusplus.py", line 252, in bundle
    bundler.update(path)
  File "/home/rayb/.local/share/pipx/venvs/competitive-verifier/lib/python3.12/site-packages/competitive_verifier/oj/verify/languages/cplusplus_bundle.py", line 483, in update
    self._resolve(pathlib.Path(included), included_from=path)
  File "/home/rayb/.local/share/pipx/venvs/competitive-verifier/lib/python3.12/site-packages/competitive_verifier/oj/verify/languages/cplusplus_bundle.py", line 291, in _resolve
    raise BundleErrorAt(path, -1, "no such header")
competitive_verifier.oj.verify.languages.cplusplus_bundle.BundleErrorAt: algo/common.h: line -1: no such header
