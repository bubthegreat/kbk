# tiltfile
allow_k8s_contexts('default')

# Define the services using Docker Compose
docker_build('kbk-sql', 'mysql/')
docker_build('kbk', 
    context='./',
    dockerfile='Dockerfile',
    live_update=[
        sync('./area/', '/kbk/area/'),
        sync('./src/', '/kbk/src/'),
        sync('./player/', '/kbk/player/'),
    ]
)

# Load the Kubernetes manifests
k8s_yaml('k8s/mysql-deployment.yaml')
k8s_yaml('k8s/kbk-deployment.yaml')


k8s_resource(workload='kbk-deployment', port_forwards=8989)
k8s_resource(workload='mysql-deployment', port_forwards=3306)
