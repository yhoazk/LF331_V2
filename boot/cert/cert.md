# Certificates

Some bootloaders need certificates which contain "magic" references and
values, which need specific offsets and addresses. In this case gcc + 
linker loader can be used.

The variables need to appear in the C file as in the LD file.
Otherwise they are ignored w/o warning.