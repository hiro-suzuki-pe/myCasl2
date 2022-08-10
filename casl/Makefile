# (1)コンパイラ
CC  = gcc
YACC	= bison -d
LEX		= flex

# (2)コンパイルオプション
CFLAGS    = -w #  -Wall

# (3)実行ファイル名
TARGET  = casl

# (4)コンパイル対象のソースコード
SRCS    = casl.c casl.tab.c lex.yy.c message.c 

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
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS)

# (10)オブジェクトファイル生成
.c.o:	
	$(CC) $(CFLAGS) $(INCDIR)	-c $<
casl.tab.c:	casl.y
	$(YACC) $?
lex.yy.c: casl.l casl.tab.h
	$(LEX) $?

# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(OBJS) $(TARGET)

# (12).oファイル、実行ファイル、.dファイルを削除
clean:
	del $(OBJS) $(TARGET) casl.tab.c casl.tab.h lex.yy.c
