ODIR = ./build
output_folder := $(shell mkdir -p $(ODIR))

all: seadsa

seadsa:
  @echo Configuring...
  @cd $(ODIR) && cmake ..
  @echo Building...
  @$(MAKE) -C $(ODIR)

clean:
  @echo Cleaning up...
  @rm -rf $(ODIR)
