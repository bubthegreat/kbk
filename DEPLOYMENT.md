# KBK Deployment Guide

This document provides a quick reference for deploying KBK (Kill or be Killed) MUD server.

## Repository Structure

```
kbk/
├── argocd/                          # ArgoCD ApplicationSet manifests
│   ├── kbk-applicationset.yaml      # Deploys all environments
│   ├── ingress-nginx-tcp-config.yaml # TCP routing config
│   └── README.md                    # ArgoCD setup guide
├── k8s/                             # Kubernetes manifests
│   ├── base/                        # Base Kustomize manifests
│   ├── overlays/                    # Environment-specific overlays
│   │   ├── prod/                    # Production
│   │   ├── kbk-staging/             # Staging
│   │   └── dev/                     # Development
│   ├── ingress-nginx/               # ingress-nginx TCP config
│   └── README.md                    # Kubernetes deployment guide
├── src/                             # C source code
├── area/                            # MUD area files
├── player/                          # Player data
├── Dockerfile                       # Container image
└── README.md                        # Main project README
```

## Quick Start

### Prerequisites

1. Kubernetes cluster (homelab-k3s)
2. ArgoCD installed
3. ingress-nginx controller
4. MetalLB for LoadBalancer
5. Storage class (Longhorn)

### Deploy with ArgoCD (Recommended)

```bash
# 1. Push code to GitHub
git remote set-url origin https://github.com/bubthegreat/kbk.git
git push -u origin master

# 2. Deploy App of Apps (single command!)
kubectl apply -f argocd/kbk-app-of-apps.yaml

# 3. Monitor deployment
kubectl get applications -n argocd | grep kbk
```

The App of Apps will automatically create:
- `kbk-environments` (ApplicationSet)
- `kbk-prod`, `kbk-staging`, `kbk-dev` (Applications)
- `kbk-ingress-nginx-tcp-config` (Application)

### Deploy Manually

```bash
# Deploy all environments
kubectl apply -k k8s/overlays/prod
kubectl apply -k k8s/overlays/kbk-staging
kubectl apply -k k8s/overlays/dev

# Deploy TCP routing config
kubectl apply -f k8s/ingress-nginx/tcp-services-configmap.yaml

# Restart ingress-nginx to pick up TCP config
kubectl rollout restart deployment -n ingress-nginx ingressnginx-ingress-nginx-controller
```

## Environments

| Environment | Namespace | Telnet | HTTP |
|-------------|-----------|--------|------|
| Production | `kbk-prod` | `telnet 192.168.0.20 8989` | `http://kbk.bubtaylor.com` |
| Staging | `kbk-staging` | `telnet 192.168.0.20 8988` | `http://kbk-staging.bubtaylor.com` |
| Development | `kbk-dev` | `telnet 192.168.0.20 8987` | `http://kbk-dev.bubtaylor.com` |

## Common Tasks

### Check deployment status
```bash
kubectl get pods -n kbk-prod
kubectl get pods -n kbk-staging
kubectl get pods -n kbk-dev
```

### View logs
```bash
# KBK server
kubectl logs -n kbk-prod deployment/kbk-deployment -f

# MySQL
kubectl logs -n kbk-prod deployment/mysql-deployment -f
```

### Restart a deployment
```bash
kubectl rollout restart deployment -n kbk-prod kbk-deployment
```

### Access MySQL directly
```bash
# Port-forward to MySQL
kubectl port-forward -n kbk-prod svc/kbk-sql 3306:3306

# Connect with mysql client
mysql -h 127.0.0.1 -u kbkuser -pkbkpassword
```

### Update an environment
```bash
# If using ArgoCD, just push to GitHub
git add .
git commit -m "Update configuration"
git push

# ArgoCD will auto-sync within ~3 minutes
# Or force sync immediately:
argocd app sync kbk-prod

# If deploying manually
kubectl apply -k k8s/overlays/prod
```

## NAT Configuration

To expose telnet ports externally, configure your router/firewall to forward:

| External Port | Internal IP | Internal Port | Service |
|---------------|-------------|---------------|---------|
| 8989 | 192.168.0.20 | 8989 | Production |
| 8988 | 192.168.0.20 | 8988 | Staging |
| 8987 | 192.168.0.20 | 8987 | Development |

## Troubleshooting

### Pods not starting
```bash
# Check events
kubectl describe pod -n kbk-prod <pod-name>

# Check PVC status
kubectl get pvc -n kbk-prod
```

### Telnet connection refused
```bash
# Verify TCP ConfigMap
kubectl get configmap -n ingress-nginx tcp-services -o yaml

# Check ingress-nginx service
kubectl get svc -n ingress-nginx ingressnginx-ingress-nginx-controller

# Restart ingress-nginx
kubectl rollout restart deployment -n ingress-nginx ingressnginx-ingress-nginx-controller
```

### MySQL connection errors
```bash
# Check MySQL is running
kubectl get pods -n kbk-prod | grep mysql

# Check MySQL logs
kubectl logs -n kbk-prod deployment/mysql-deployment

# The KBK server waits for MySQL - this is normal during startup
```

### ArgoCD sync issues
```bash
# Check application status
argocd app get kbk-prod

# View diff
argocd app diff kbk-prod

# Force sync
argocd app sync kbk-prod --force

# Refresh from Git
argocd app get kbk-prod --refresh
```

## Building and Pushing Images

```bash
# Build images
make build

# Push to registry
make push

# Deploy to cluster
make deploy
```

See the [Makefile](Makefile) for available targets.

## Documentation

- [ArgoCD Setup](argocd/README.md) - Detailed ArgoCD configuration
- [Kubernetes Deployment](k8s/README.md) - Kubernetes manifest details
- [Main README](README.md) - Project overview and local development

## Support

For issues or questions:
1. Check the troubleshooting sections above
2. Review logs: `kubectl logs -n <namespace> deployment/<deployment-name>`
3. Check ArgoCD UI for sync status
4. Review the detailed READMEs in `argocd/` and `k8s/` directories

