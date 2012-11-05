program-project0: $(BINDIR)/project0.bin
		@echo -e "[PROG]\t" project0
		$(LMFLASH) $(BINDIR)/project0.bin
