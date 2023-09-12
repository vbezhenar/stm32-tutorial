.PHONY: serve build
serve build: | venv
	$(VENV)/mkdocs $@

.INTERMEDIATE: pages/README.md
serve build: pages/README.md
pages/README.md: README.md
	cp $< $@

include Makefile.venv
Makefile.venv:
	curl \
		-o Makefile.fetched \
		-L "https://github.com/sio/Makefile.venv/raw/v2023.04.17/Makefile.venv"
	echo "fb48375ed1fd19e41e0cdcf51a4a0c6d1010dfe03b672ffc4c26a91878544f82 *Makefile.fetched" \
		| sha256sum --check - \
		&& mv Makefile.fetched Makefile.venv
