build:
	docker build -t bubthegreat/kbk:latest .

push: build
	docker push bubthegreat/kbk:latest

deploy: push
	kubectl config use-context default
	kubectl apply -k k8s/base
