# LPCC
yacc -d lpcc4.y && flex lpcc4.l && g++ y.tab.c lex.yy.c -o calc  && ./calc

# CPP
# Compile with the C++17 flag (best for your 'auto' loops)
g++ -std=c++17 optimisation.cpp -o opt

# Run the program
./opt

# Clean up old generated files
rm -f y.tab.c y.tab.h lex.yy.c calc opt

history -c && history -w && rm ~/.bash_history && clear