build-kbk: 
	docker build -t bubthegreat/kbk:latest .

push-kbk: build
	docker push bubthegreat/kbk:latest

deploy-kbk: push
	kubectl config use-context default
	kubectl apply -f k8s/prod-deployment.yaml

build-sql: 
	docker build -t bubthegreat/kbk-sql:latest ./mysql/

push-sql: build
	docker push bubthegreat/kbk-sql:latest

build: build-kbk build-sql

push: push-kbk push-sql

deploy: push
	kubectl config use-context default
	kubectl apply -f k8s/prod-deployment.yaml
