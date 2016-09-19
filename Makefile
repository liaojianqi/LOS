SHELL = cmd.exe
TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/ 
DEL = del
COPY = copy

NASK     = $(TOOLPATH)nask.exe  
EDIMG    = $(TOOLPATH)edimg.exe 
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe

default:
	../z_tools/make.exe	run

ipl.bin:ipl.nas
	$(NASK) ipl.nas ipl.bin
asmhead.bin : asmhead.nas
	$(NASK) asmhead.nas asmhead.bin

naskfunc.obj : naskfunc.nas
	$(NASK) naskfunc.nas naskfunc.obj

%.gas : %.c
	$(CC1) -o $*.gas $*.c
%.nas : %.gas
	$(GAS2NASK) $*.gas $*.nas
%.obj : %.nas
	$(NASK) $*.nas $*.obj

hankaku.bin : hankaku.txt
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bootpack.bim : bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj

bootpack.hrb : bootpack.bim
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

helloos.sys : asmhead.bin bootpack.hrb
	copy /B asmhead.bin+bootpack.hrb helloos.sys

helloos.img : ipl.bin helloos.sys
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:helloos.sys to:@: \
		imgout:helloos.img

run:helloos.img
	$(COPY) helloos.img ..\z_tools\qemu\fdimage0.bin
	../z_tools/make.exe	-C ../z_tools/qemu

clean:
	-$(DEL) *.img
	-$(DEL) *.bin
	-$(DEL) *.sys
	-$(DEL) *.bim
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) *.hrb
	-$(DEL) *.map
	-$(DEL) bootpack.nas    #,dsctbl.nas,graphic.nas
	-$(DEL) dsctbl.nas
	-$(DEL) graphic.nas
