#!/bin/sh
set -e -x

# Setup workspace for qt-creator
# Creates "project" config files

die() {
  echo "$1"
  exit 1
}

# Find all directories containing headers
findinc() {
  git ls-files|egrep '\.h$|\.cpp$'| xargs -l1 dirname|sort -u
}

[ -f ./RELEASE.local ] || die "Execute from the wrapper repo top"

ROOT="$PWD"

for repo in epics-base pvData pvAccess pvaSrv normativeTypes pvaClient
do
  [ -d "$repo" ] || continue

  cat << EOF > "$repo/$repo.creator"
[General]
EOF

  cat <<EOF > "$repo/$repo.config"
EOF

  (cd "$repo" && git ls-files) > "$repo/$repo.files"

  # first pass for .includes
  (cd "$repo" && findinc) > "$repo/$repo.includes"

  if ! grep "^$repo\.\*$" ".git/modules/$repo/info/exclude"
  then
    cat << EOF >> ".git/modules/$repo/info/exclude"
$repo.*

EOF

    cat "$repo/$repo.includes" >> ".git/modules/$repo/info/exclude"
  fi
done

# merge .includes for dependent packages

mergeme() {
   output="$1"
   shift
   echo "Module $output depends on $@"

   for repo in "$@"
   do
       cat "$repo/$repo.includes" \
       | while read dir; do if expr match "$dir" "^/" >/dev/null; \
          then echo "$dir"; \
          else echo "$ROOT/$repo/$dir"; fi; done \
       >> "$output/$output.includes"
   done
}

mergeme pvData epics-base
mergeme pvAccess pvData epics-base
mergeme pvaSrv pvAccess pvData epics-base
mergeme normativeTypes pvAccess pvData epics-base
mergeme pvaClient normativeTypes pvAccess pvData epics-base
