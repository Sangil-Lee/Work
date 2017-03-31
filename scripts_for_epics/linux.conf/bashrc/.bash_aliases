#
# 
# if [ -f ~/.bash_aliases ]; then
#    . ~/.bash_aliases
# fi


alias vi='vim'
alias du='du -h'
alias df='df -kh'


#-------------------------------------------------------------
# The 'ls' family (this assumes you use a recent GNU ls).
#-------------------------------------------------------------
# Add colors for filetype and  human-readable sizes by default on 'ls':
alias ls='ls -h --color'
alias lx='ls -lXB'         #  Sort by extension.
alias lk='ls -lSr'         #  Sort by size, biggest last.
alias lt='ls -ltr'         #  Sort by date, most recent last.
alias lc='ls -ltcr'        #  Sort by/show change time,most recent last.
alias lu='ls -ltur'        #  Sort by/show access time,most recent last.

# The ubiquitous 'll': directories first, with alphanumeric sorting:
alias ll="ls -lv --group-directories-first"
alias lm='ll |more'        #  Pipe through 'more'
alias lr='ll -R'           #  Recursive ls.
alias la='ll -A'           #  Show hidden files.
alias tree='tree -Csuh'    #  Nice alternative to 'recursive ls' ...


alias dir='dir --color=auto'
alias vdir='vdir --color=auto'

alias grep='grep --color=auto'
alias fgrep='fgrep --color=auto'
alias egrep='egrep --color=auto'

alias mv='mv -i'
alias cp='cp -i'

alias watchtime='watch --interval 1 date'

alias p5="ps -eo user,pcpu,pid,cmd | sort -r -k2 | head -6"


# Pretty-print of some PATH variables:
alias path='echo -e ${PATH//:/\\n}'
alias libpath='echo -e ${LD_LIBRARY_PATH//:/\\n}'


alias epicsenv=". ~/epics/R3.14.12.5/setEpicsEnv.sh"



goEpics() {

    if [ -z ${EPICS_PATH} ]; then
	echo "No EPICS environment variables are found. Force to set the default one."
	echo "~/epics/R3.14.12.5/setEpicsEnv.sh"
	echo ""
	. ~/epics/R3.14.12.5/setEpicsEnv.sh
    fi

    if [[ $# -eq 0 ]] ; then
	cd ${EPICS_PATH}
    else
	cd ${EPICS_PATH}
	cd $1
    fi
}



goSynApps() {

    if [ -z ${EPICS_SYNAPPS} ]; then
	echo "No EPICS environment variables are found. Force to set the default one."
	echo "~/epics/R3.14.12.5/setEpicsEnv.sh"
	echo ""
	. ~/epics/R3.14.12.5/setEpicsEnv.sh
    fi

    if [[ $# -eq 0 ]] ; then
	cd ${EPICS_SYNAPPS}
    else
	cd ${EPICS_SYNAPPS}
	cd $1
    fi
}



goApps() {

    if [ -z ${RAON_SITEAPPS} ]; then
	echo "No EPICS environment variables are found. Force to set the default one."
	echo "~/epics/R3.14.12.5/setEpicsEnv.sh"
	echo ""
	. ~/epics/R3.14.12.5/setEpicsEnv.sh
    fi

    if [[ $# -eq 0 ]] ; then
	cd ${RAON_SITEAPPS}
    else
	cd ${RAON_SITEAPPS}
	cd $1
    fi
}


goLibs() {

    if [ -z ${RAON_SITELIBS} ]; then
	echo "No EPICS environment variables are found. Force to set the default one."
	echo "~/epics/R3.14.12.5/setEpicsEnv.sh"
	echo ""
	. ~/epics/R3.14.12.5/setEpicsEnv.sh
    fi

    if [[ $# -eq 0 ]] ; then
	cd ${RAON_SITELIBS}
    else
	cd ${RAON_SITELIBS}
	cd $1
    fi
}


goIoc() {
	goApps $1
	cd iocBoot/ioc$1
}



runIoc() {

#    local ioc_name=$1

    if [[ $# -eq 0 ]] ; then
	cd ${RAON_SITEAPPS}
    else   
        goIoc $1
    fi
#    goIoc ${ioc_name}

    case `uname -sm` in
	"Linux i386" | "Linux i486" | "Linux i586" | "Linux i686")
	    EPICS_HOST_ARCH=linux-x86
	    ;;
	"Linux x86_64")
	    EPICS_HOST_ARCH=linux-x86_64
	    ;;
	"Linux armv6l")
	    EPICS_HOST_ARCH=linux-arm
	    ;;
	"Linux armv7l")
	    EPICS_HOST_ARCH=linux-arm
	    ;;
	*)
	    echo "This doesn't support this architecture : `uname -sm`"
	    exit 1
	    ;;
    esac
    
    ../../bin/${EPICS_HOST_ARCH}/$1 st.cmd
 	 
}

#
# Global EXPORT setup
#


export EPICS_CA_AUTO_ADDR_LIST=yes


