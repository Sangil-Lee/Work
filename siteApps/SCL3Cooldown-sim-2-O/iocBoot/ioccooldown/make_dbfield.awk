
rm -f ../../cooldownApp/Db/$2
awk 'BEGIN \
{ i=0; print "\
record(ai, $(SYS)$(SUBSYS)FwdLnk) {\
		\n\tfield(SCAN, \"Passive\")\
		\n\tfield(FLNK, \"$(SYS)$(SUBSYS)Fanout1\")\
	\n}\n\
record(fanout, $(SYS)$(SUBSYS)Fanout1){\
		\n\tfield(SCAN, \"Passive\")\
		\n\tfield(LNK0, \"$(SYS)$(SUBSYS)Fanout1-1\")\
		\n\tfield(LNK1,\"$(SYS)$(SUBSYS)Fanout1-2\")\
		\n\tfield(LNK2,\"$(SYS)$(SUBSYS)Fanout1-3\")\
		\n\tfield(LNK3,\"$(SYS)$(SUBSYS)Fanout1-4\")\
		\n\tfield(LNK4,\"$(SYS)$(SUBSYS)Fanout1-5\")\
	\n}\n"\
}
NR%16==1 { print "#"int(NR/16)"\n" \
	"record(fanout,$(SYS)$(SUBSYS)Fanout1-" int(NR/16)+1 ") {\
		\n\tfield(SCAN, \"Passive\")" 
} 
{ printf "\tfield(LNK%X,\"%s\")\n", i++%16, $0 } 
int(NR%16)==0 { print "}\n" }
END {print "}\n"}' \
$1 >> ../../cooldownApp/Db/$2

#string substitution ex.
#awk '{sub(/are hello/,"are are")sub(/you hello/,"you you")}1' file
#/foo/bar/how /SOME_TEXT_HERE/hello
#/foo/bar/are are/SOME_OTHER_TEXT
#/foo/bar/you you

