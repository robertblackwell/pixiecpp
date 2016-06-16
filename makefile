SRC=external_src

DEPS=deps

REPO_DIR=$(HOME)/git-repos

TWC=twc.c twc.h

SOCKFUNC=socket_functions.h socket_functions.cpp

SIMPLEBUFFER=simple_buffer.h simple_buffer.c

HTTPMESSAGE=

HTTP_PARSER_C=http_parser.c http_parser.h

$(DEPS):
	mkdir $(DEPS)

$(SRC):
	mkdir $(SRC)
	
easyloggingpp:
	git clone $(REPO_DIR)/easyloggingpp $(DEPS)/easyloggingpp
	-cp $(DEPS)/easyloggingpp/src/*.h  $(SRC)/
	-cp -R $(DEPS)/easyloggingpp/src/plog  $(SRC)/ 
	
twx:
	git clone $(REPO_DIR)/twc $(DEPS)/twc
	-cp $(DEPS)/twc/src/*.c  $(SRC)/
	-cp $(DEPS)/twc/src/*.h  $(SRC)/

simple_buffer:
	git clone $(REPO_DIR)/simple_buffer $(DEPS)/simple_buffer
	-cp $(DEPS)/simple_buffer/src/*.c  $(SRC)/
	-cp $(DEPS)/simple_buffer/src/*.h  $(SRC)/

http_message:
	git clone $(REPO_DIR)/HTTPMessage $(DEPS)/HTTPMessage
	-cp $(DEPS)/HTTPMessage/src/*.c  $(SRC)/
	-cp $(DEPS)/HTTPMessage/src/*.cpp  $(SRC)/
	-cp $(DEPS)/HTTPMessage/src/*.h  $(SRC)/
	-cp $(DEPS)/HTTPMessage/src/*.hpp  $(SRC)/
		

install_deps: $(SRC) $(DEPS) twx simple_buffer http_message easyloggingpp
	
	
clean_deps:	
	rm -Rf $(SRC)/*
	rm -Rf $(DEPS)/*