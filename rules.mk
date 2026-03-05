AUTO_SHIFT_ENABLE = yes
CAPS_WORD_ENABLE = yes

# Customise bootmagic.

ifeq ($(strip $(BOOTMAGIC_ENABLE)), yes)
  SRC += custom_keys_bootmagic.c
endif
