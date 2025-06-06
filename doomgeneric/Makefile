################################################################
#
# $Id:$
#
# $Log:$
#

ifeq ($(V),1)
	VB=''
else
	VB=@
endif


CC=gcc
CFLAGS+=-g -Os -std=c11 -ffunction-sections
LDFLAGS+=-Wl,--gc-sections #,--print-gc-sections
CFLAGS+=-g -DNORMALUNIX -DLINUX -DSNDSERV -D_DEFAULT_SOURCE # -DUSEASM
LIBS+=-lm -lc -lX11

# subdirectory for objects
OBJDIR=build
OUTPUT=doomgeneric

SRC_DOOM = am_map.o doomdef.o doomstat.o dstrings.o d_event.o d_items.o d_iwad.o d_loop.o d_main.o d_net.o f_finale.o f_wipe.o g_game.o hu_lib.o hu_stuff.o info.o i_scale.o i_sound.o i_system.o i_timer.o m_argv.o m_bbox.o m_cheat.o m_config.o m_controls.o m_fixed.o m_menu.o m_misc.o m_random.o p_ceilng.o p_doors.o p_enemy.o p_floor.o p_inter.o p_lights.o p_map.o p_maputl.o p_mobj.o p_plats.o p_pspr.o p_setup.o p_sight.o p_spec.o p_switch.o p_telept.o p_tick.o p_user.o r_bsp.o r_data.o r_draw.o r_main.o r_plane.o r_segs.o r_sky.o r_things.o sounds.o st_lib.o st_stuff.o s_sound.o tables.o v_video.o wi_stuff.o w_file.o w_wad.o z_zone.o i_input.o glue.o i_video.o doomgeneric.o doomgeneric_xlib.o
OBJS += $(addprefix $(OBJDIR)/, $(SRC_DOOM))

all:	 $(OUTPUT)

single_c:
	gcc -o doom -Os -std=c11 everything.c doomgeneric_xlib.c -lX11

doom64:
	~/git/GCC/install_doom64/bin/doom64-unknown-elf-gcc -Os -std=gnu11 everything.c -DDOOM64 -S -fno-strict-aliasing

clean:
	rm -rf $(OBJDIR)
	rm -f $(OUTPUT)
	rm -f $(OUTPUT).gdb
	rm -f $(OUTPUT).map

$(OUTPUT):	$(OBJS)
	@echo [Linking $@]
	$(VB)$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) \
	-o $(OUTPUT) $(LIBS) -Wl,-Map,$(OUTPUT).map
	@echo [Size]
	-$(CROSS_COMPILE)size $(OUTPUT)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o:	%.c
	@echo [Compiling $<]
	$(VB)$(CC) $(CFLAGS) -c $< -o $@

print:
	@echo OBJS: $(OBJS)

