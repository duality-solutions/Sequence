.PHONY: FORCE
all: FORCE
	$(MAKE) -C .. sequence_qt test_sequence_qt
clean: FORCE
	$(MAKE) -C .. sequence_qt_clean test_sequence_qt_clean
check: FORCE
	$(MAKE) -C .. test_sequence_qt_check
sequence-qt sequence-qt.exe: FORCE
	 $(MAKE) -C .. sequence_qt
