CXXFLAGS=-I. -Itest -Wall -pedantic -Wno-long-long

all:
	@echo If you want to test, please type "'make test'"

clean:
	rm -rf *.o *.obj *.exe

test: testrunner
	./testrunner --log_level=warning

test/test_packrat.o: any_key.hpp typeindex.hpp fusion_hash.hpp packrat.hpp packrat_core.hpp packrat_rule.hpp packrat_operators.hpp test/test_packrat_helper.hpp
test/test_packrat.o: packrat_grammar.hpp
test/test_packrat_auxiliary.o: any_key.hpp typeindex.hpp fusion_hash.hpp packrat.hpp packrat_core.hpp packrat_rule.hpp packrat_operators.hpp test/test_packrat_helper.hpp
test/test_packrat_binary.o: any_key.hpp typeindex.hpp fusion_hash.hpp packrat.hpp packrat_core.hpp packrat_rule.hpp packrat_operators.hpp test/test_packrat_helper.hpp
test/test_packrat_char.o: any_key.hpp typeindex.hpp fusion_hash.hpp packrat.hpp packrat_core.hpp packrat_rule.hpp packrat_operators.hpp test/test_packrat_helper.hpp
test/test_packrat_directive.o: any_key.hpp typeindex.hpp fusion_hash.hpp packrat.hpp packrat_core.hpp packrat_rule.hpp packrat_operators.hpp test/test_packrat_helper.hpp
test/test_packrat_helper.o: test/test_packrat_helper.hpp any_key.hpp typeindex.hpp fusion_hash.hpp
test/test_packrat_numeric.o: any_key.hpp typeindex.hpp fusion_hash.hpp packrat.hpp packrat_core.hpp packrat_rule.hpp packrat_operators.hpp test/test_packrat_helper.hpp
test/test_packrat_operator.o: any_key.hpp typeindex.hpp fusion_hash.hpp packrat.hpp packrat_core.hpp packrat_rule.hpp packrat_operators.hpp test/test_packrat_helper.hpp
test/test_packrat_string.o: any_key.hpp typeindex.hpp fusion_hash.hpp packrat.hpp packrat_core.hpp packrat_rule.hpp packrat_operators.hpp test/test_packrat_helper.hpp
test/test_any_key.o: any_key.hpp
test/test_typeindex.o: typeindex.hpp
test/test_fusion_hash.o: fusion_hash.hpp

testrunner: $(patsubst test%.cpp,test%.o,$(wildcard test/test*.cpp))
	$(CXX) $(CXXFLAGS) -o $@ $^
