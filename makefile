# Makefile The Boys
# Marcello Eduardo, 2024/2

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99
LDLIBS  = -lm
MAIN    = theboys

# lista de arquivos de cabeçalho
HDR = lista.h fprio.h conjunto.h entidades.h eventos.h fila.h mundo.h complementos.h

# lista de arquivos-objeto
OBJ = lista.o fprio.o entidades.o eventos.o fila.o mundo.o complementos.o theboys.o

# conjunto.o é precompilado, adicionado diretamente às dependências
EXTERNAL_OBJ = conjunto.o

# construir o executável
$(MAIN): $(OBJ) $(EXTERNAL_OBJ)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJ) $(EXTERNAL_OBJ) $(LDLIBS)

# regras para construir os arquivos-objeto
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $<

# executar
run: $(MAIN)
	./$(MAIN)

# testar no Valgrind
valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)

# gerar arquivo TGZ para entrega
tgz: clean
	-mkdir -p /tmp/$(USER)/$(MAIN)
	chmod 0700 /tmp/$(USER)/$(MAIN)
	cp *.c *.h makefile /tmp/$(USER)/$(MAIN)
	tar czvf $(MAIN).tgz -C /tmp/$(USER) $(MAIN)
	rm -rf /tmp/$(USER)
	@echo "Arquivo $(MAIN).tgz criado para entrega"

# limpar arquivos temporários
clean:
	rm -f *~ $(OBJ) $(MAIN) $(MAIN).tgz
