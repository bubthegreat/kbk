CC		= gcc
PROF	= -g
NOCRYPT	=
C_FLAGS = -Wall $(PROF) $(NOCRYPT)
L_FLAGS =  $(PROF)
E_FLAGS = `mysql_config --cflags --libs` -lcrypt

O_FILES = act_comm.o act_enter.o act_info.o act_move.o act_obj.o act_wiz.o \
	alias.o ban.o comm.o const.o db.o db2.o effects.o fight.o \
	handler.o healer.o interp.o lookup.o magic.o recycle.o \
	save.o skills.o special.o tables.o update.o morecabal.o morefight.o \
	extra.o moremagic.o titles.o necro.o olc.o olc_act.o \
	assassin.o olc_save.o mem.o string.o bit.o commune.o tracerah.o \
	iprog.o mprog.o rprog.o bounty.o mysql.o newbit.o ranger.o \
	note.o queststore.o

pos2: $(O_FILES)
	rm -f pos2
	$(CC) -I/usr/include/mysql $(L_FLAGS) -o pos2 $(O_FILES) $(E_FLAGS)
	mv pos2 /kbk/area
	cd ..

.c.o: merc.h
	$(CC) -I/usr/include/mysql -c $(C_FLAGS) $<

clean:
	rm -f *.o

backup:
	@tar -cpf `date +%m.%d.%y-%I%M`.src.tar.gz -z *.c *.h
	@mv *.gz /kbk/backups
	@echo "Made backup of source in /kbk/backups"

areabackup:
	@tar -cpf `date +%m.%d.%y-%I%M`.areas.tar.gz -z /kbk/area/*.are /kbk/area/*.txt /kbk/area/*.hlp /kbk/area/*.lst
	@mv *.gz /kbk/backups
	@echo "Made backup of areas in /kbk/backups"

fullbackup:
	@tar -cpf `date +%m.%d.%y-%I%M`.full.tar.gz -z /kbk/src/*.c /kbk/src/*.h /kbk/src/Makefile /kbk/area/*.are /kbk/area/*.txt /kbk/area/*.lst /kbk/area/*.hlp /kbk/player/*.*
	@mv *.gz /kbk/backups
	@echo "Made full backup of players, areas, and source in /kbk/backups"

playerbackup:
	@tar -cpf `date +%m.%d.%y-%I%M`.player.tar.gz -z /kbk/player/*.*
	@mv *.gz /kbk/backups
	@echo "Made backup of player files in /kbk/backups"
