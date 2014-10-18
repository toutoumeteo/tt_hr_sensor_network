# .bash_profile

#==============================================================================
ulimit -S -m 1000000
#==============================================================================

export EDITOR=emacs

alias lsr='ls -lrt'
alias h='history' 
alias rm='rm -i' 
alias ll='ls -al | more' 

function emacs {
   /usr/bin/emacs $* -bg white -font -adobe-courier-medium-r-normal--20-140-10-100-m-110-iso10646-1
}

function e {
   /usr/bin/emacs $* -bg white -font -adobe-courier-medium-r-normal--20-140-10-100-m-110-iso10646-1
}

