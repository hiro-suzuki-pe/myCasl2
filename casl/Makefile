# (1)コンパイラ
CC  = gcc
YACC	= bison -d
LEX		= flex

# (2)コンパイルオプション
CFLAGS    = -w -g -DYY_SKIP_YYWRAP #  -Wall

# (3)実行ファイル名
TARGET  = casl.exe

# (4)コンパイル対象のソースコード
SRCS    = caslY.tab.c lex.yy.c casl_main.c message.c 

# (5)オブジェクトファイル名
OBJS    = $(SRCS:.c=.o)

# (6)インクルードファイルのあるディレクトリパス
INCDIR  = #		-I../inc

# (7)ライブラリファイルのあるディレクトリパス
LIBDIR  = 

# (8)追加するライブラリファイル
LIBS    = 	 # -lfl  # 	-ll

# (9)ターゲットファイル生成
$(TARGET): $(OBJS)
	$(CC) -o $@ -g $^ $(LIBDIR) $(LIBS)
	$(TARGET) ../test.cas
# (10)オブジェクトファイル生成
.c.o:	
	$(CC) $(CFLAGS) $(INCDIR)	-c $<
caslY.tab.c:	caslY.y
	$(YACC) $?
lex.yy.c: casl.l caslY.tab.h
	$(LEX) $?

# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(OBJS) $(TARGET)

# (12).oファイル、実行ファイル、.dファイルを削除
clean:
	rm -f $(OBJS) $(TARGET) caslY.tab.c caslY.tab.h lex.yy.c
