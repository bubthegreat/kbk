# tiltfile
allow_k8s_contexts('docker-desktop')

# Define the services using Docker Compose
docker_build('bubthegreat/kbk-sql', 'mysql/')
docker_build('bubthegreat/kbk',
    context='./',
    dockerfile='Dockerfile',
    live_update=[
        sync('./area/', '/kbk/area/'),
        sync('./player/', '/kbk/player/'),
        sync('./src/', '/kbk/src/'),
        run('cd /kbk/src && make -j8', 
            trigger=[
                './src/act_obj.c',
            ]
        )
    ],
    ignore=[
        './localxfer/'
    ]
)

# Load the Kubernetes manifests using kustomize local overlay
k8s_yaml(kustomize('k8s/overlays/local'))

k8s_resource(workload='kbk-deployment', port_forwards=8989)
k8s_resource(workload='mysql-deployment', port_forwards=3306)
