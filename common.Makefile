INDENTSTYLE = -kr -i8

default: test

.PHONY: objclean distclean indent count 
objclean:
	find . -name '*.o' -print0 | \
		xargs -0 -r -P 20 -n 1 rm

distclean: 
	find . \( -name '*~' -or -name 'core*' \) -print0 | \
		xargs -0 -r -P 20 -n 1 rm

clean: objclean distclean
	-rm -rf $(TARGETS)

indent:
	find . \( -name '*.cpp' -or -name '*.h' \) -print0 | \
		xargs -0 -r -n 1 indent $(INDENTSTYLE)

count:
	find . \( -name '*.cpp' -or -name '*.h' \) -print0 | \
		xargs -0 -r -n 1 wc -l		
