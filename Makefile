build:
	docker build -t bubthegreat/kbk:latest .

push: build
	docker push bubthegreat/kbk:latest

deploy: push
	kubectl config use-context default
	kubectl apply -k k8s/base

install-base:
	apt install -y curl

install-tilt: install
	curl -fsSL https://raw.githubusercontent.com/tilt-dev/tilt/master/scripts/install.sh | bash

install: install-base install-tilt
	@echo "All installs complete!"

install-local:
	@echo "Installing local environment..."
	apt install -y build-essential libsqlite3-dev sqlite3 gdb libc6-dbg telnet

build-local:
	@echo "Building local environment..."
	cd src && make -j8

run-local:
	@echo "Running local environment..."
	cd area && ./start-kbk.sh

build-run: build-local run-local
	