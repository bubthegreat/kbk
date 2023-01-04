load('ext://uibutton', 'cmd_button', 'location', 'text_input')

# minikube addons enable ingress
# minikube addons enable ingress-dns
# minikube addons enable dashboard
# minikube addons enable metrics-server

# Get ingress logs: 
# kubectl logs -n ingress-nginx `kubectl get pods -n ingress-nginx | grep controller | awk '{print $1}'`

docker_build('kbk-service', 
    context='.', 
    dockerfile='Dockerfile',
    live_update=[
        # Sync files from host to container
        sync('./area/', '/kbk/area/'),
        sync('./player/', '/kbk/player/'),
        sync('./sys/', '/kbk/sys/'),
        sync('./src/', '/kbk/src/'),
        run(
            'cd /kbk/src && make -j8',
            trigger=['./src/']
        )

    ]
)
docker_prune_settings( disable = False , max_age_mins = 360 , num_builds = 0 , interval_hrs = 1 , keep_recent = 2 ) 



k8s_yaml([
    'kbk-k8s-yaml/kbk_network-networkpolicy.yaml',
    'kbk-k8s-yaml/kbk-deployment.yaml',
    'kbk-k8s-yaml/kbk-players-persistentvolumeclaim.yaml',
    'kbk-k8s-yaml/kbk-service.yaml',
])

k8s_yaml([
    'kbk-k8s-yaml/kbk-sql-data-persistentvolumeclaim.yaml',
    'kbk-k8s-yaml/kbk-sql-deployment.yaml',
    'kbk-k8s-yaml/kbk-sql-service.yaml',
    'kbk-k8s-yaml/kbk-sys-persistentvolumeclaim.yaml',
])

k8s_resource('kbk-service', port_forwards=8989, labels=['services'])
k8s_resource('kbk-sql', port_forwards=3306, labels=['services'])


local_resource('tunnel', cmd="minikube tunnel", labels=['kubernetes'], allow_parallel=True, auto_init=False)
local_resource('dashboard', cmd="minikube dashboard",  labels=['kubernetes'], auto_init=False, allow_parallel=True)

# Open the kubernetes dashboard
cmd_button(
    name='nav-dashboard',
    argv=['minikube', 'dashboard'],
    text='Minikube Dashboard',
    location=location.NAV,
    icon_name='install_desktop'
)