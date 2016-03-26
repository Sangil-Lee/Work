#!/bin/bash
#
#  Shell   : raonctrl_cloning.sh
#  Author  : Jeong Han Lee
#  email   : jhlee@ibs.re.kr
#  Date    : Sunday, July 12 17:55:32 KST 2015
#  version : 0.0.3
#
#   - 0.0.1 Tuesday, March 10 07:29:12 KST 2015, jhlee
#           * created
#
#   - 0.0.2 Monday, April  6 20:11:47 KST 2015, jhlee
#           * ask ONCE username and its password, and transfer them into two
#             git clone commands
#
#   - 0.0.3 Sunday, July 12 17:55:41 KST 2015, jhlee
#           * somehow, user and password are saved into .git
#             so, cache it 
#           * cache doesn't work according to my intention, 
#             so wipe all "used data" after cloning...
#

# This will exit your script if you try to use an uninitialised variable
set -u

git config --global credential.helper 'cache --timeout=60'

cd ${EPICS_PATH}


echo "Username for 'https://github.com/RaonControl'";
read username;


# just copy the following line codes in the "stackoverflow"
# 
# http://stackoverflow.com/questions/4316730/linux-scripting-hiding-user-input-on-terminal
#
# Monday, April  6 20:15:54 KST 2015, jhlee
#
unset password;

echo "Password for 'https://${username}@github.com/RaonControl'"

while IFS= read -r -s -n1 char; do
  [[ -z $char ]] && { printf '\n'; break; } # ENTER pressed; output \n and break.
  if [[ $char == $'\x7f' ]]; then # backspace was pressed
      # Remove last char from output variable.
      [[ -n $password ]] && password=${password%?}
      # Erase '*' to the left.
      printf '\b \b' 
  else
    # Add typed char to output variable.
    password+=$char
    # Print '*' in its stead.
    printf '*'
  fi
done

# while IFS= read -r -s -n1 pass; do
#   if [[ -z $pass ]]; then
#      echo
#      break
#   else
#      echo -n '*'
#      password+=$pass
#   fi
# done

git clone https://${username}:${password}@github.com/RaonControl/siteLibs

# go to siteLibs

cd siteLibs 
#
# Replace the saved user:pass url with a simple github.com url
#
git remote set-url origin https://github.com/RaonControl/siteLibs
#
# Remove all saved records in logs
#
rm .git/logs/refs/heads/master
rm .git/logs/refs/remotes/origin/HEAD
rm .git/logs/HEAD

# 
# back to the working directory
# 

cd ..
git clone https://${username}:${password}@github.com/RaonControl/siteApps

cd siteApps

git remote set-url origin https://github.com/RaonControl/siteApps
rm .git/logs/refs/heads/master
rm .git/logs/refs/remotes/origin/HEAD
rm .git/logs/HEAD

unset password;
unset username;


exit
