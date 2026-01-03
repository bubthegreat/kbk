# KBK Kubernetes Deployment

This directory contains Kustomize-based Kubernetes manifests for deploying KBK (Kill or be Killed) MUD server.

## Structure

```
k8s/
├── base/                    # Base manifests (shared across all environments)
│   ├── kbk-deployment.yaml
│   ├── kbk-service-http.yaml
│   ├── kbk-ingress.yaml
│   ├── kbk-pvc.yaml
│   ├── mysql-deployment.yaml
│   ├── mysql-service.yaml
│   ├── mysql-pvc.yaml
│   ├── mysql-secret.yaml
│   └── kustomization.yaml
├── overlays/
│   ├── prod/                # Production environment
│   ├── kbk-staging/         # Staging environment
│   └── dev/                 # Development environment
└── ingress-nginx/
    └── tcp-services-configmap.yaml  # TCP routing configuration
```

## Environments

### Production
- **Namespace**: `kbk-prod`
- **Telnet**: `telnet 192.168.0.20 8989` or `telnet kbk.bubtaylor.com 8989`
- **HTTP**: `http://kbk.bubtaylor.com`
- **Deploy**: `kubectl apply -k k8s/overlays/prod`

### Staging
- **Namespace**: `kbk-staging`
- **Telnet**: `telnet 192.168.0.20 8988` or `telnet kbk-staging.bubtaylor.com 8988`
- **HTTP**: `http://kbk-staging.bubtaylor.com`
- **Deploy**: `kubectl apply -k k8s/overlays/kbk-staging`

### Development
- **Namespace**: `kbk-dev`
- **Telnet**: `telnet 192.168.0.20 8987` or `telnet kbk-dev.bubtaylor.com 8987`
- **HTTP**: `http://kbk-dev.bubtaylor.com` or `http://kbk.localhost`
- **Deploy**: `kubectl apply -k k8s/overlays/dev`

## Architecture

### TCP Routing via ingress-nginx
All telnet connections are routed through the existing ingress-nginx LoadBalancer (192.168.0.20) using TCP port mapping:
- Port 8989 → kbk-prod/kbk-service:8989
- Port 8988 → kbk-staging/kbk-service:8989
- Port 8987 → kbk-dev/kbk-service:8989

This is configured in `k8s/ingress-nginx/tcp-services-configmap.yaml`.

### HTTP Routing
Standard Kubernetes Ingress with hostname-based routing:
- `kbk.bubtaylor.com` → kbk-prod
- `kbk-staging.bubtaylor.com` → kbk-staging
- `kbk-dev.bubtaylor.com` / `kbk.localhost` → kbk-dev

## Prerequisites

1. **Kubernetes cluster** with kubectl configured (context: `homelab-k3s`)
2. **ingress-nginx** controller installed
3. **MetalLB** for LoadBalancer services
4. **Longhorn** or another storage class for PVCs

## Deployment

### Option 1: ArgoCD (Recommended for GitOps)

See the [argocd/README.md](../argocd/README.md) for full details.

```bash
# Deploy all environments via ApplicationSet
kubectl apply -f argocd/kbk-applicationset.yaml

# Deploy ingress-nginx TCP config
kubectl apply -f argocd/ingress-nginx-tcp-config.yaml
```

ArgoCD will automatically sync changes from the GitHub repository.

### Option 2: Manual deployment with kubectl

```bash
# Production
kubectl apply -k k8s/overlays/prod

# Staging
kubectl apply -k k8s/overlays/kbk-staging

# Development
kubectl apply -k k8s/overlays/dev

# ingress-nginx TCP config
kubectl apply -f k8s/ingress-nginx/tcp-services-configmap.yaml
```

### Check status
```bash
# List all KBK resources
kubectl get all,pvc,ingress -n kbk-prod
kubectl get all,pvc,ingress -n kbk-staging
kubectl get all,pvc,ingress -n kbk-dev

# Check ingress-nginx service
kubectl get svc -n ingress-nginx ingressnginx-ingress-nginx-controller
```

### View logs
```bash
# KBK server logs
kubectl logs -n kbk-prod deployment/kbk-deployment -f

# MySQL logs
kubectl logs -n kbk-prod deployment/mysql-deployment -f
```

## Adding New Ports to NAT

If you need to expose these ports externally, add NAT forwarding rules for:
- **8989** (Production)
- **8988** (Staging)
- **8987** (Development)

All pointing to your cluster's ingress-nginx LoadBalancer IP: `192.168.0.20`

## Troubleshooting

### Telnet connection refused
Check that the tcp-services ConfigMap is applied and ingress-nginx controller has restarted:
```bash
kubectl get configmap -n ingress-nginx tcp-services -o yaml
kubectl rollout restart deployment -n ingress-nginx ingressnginx-ingress-nginx-controller
```

### MySQL connection errors
The KBK server waits for MySQL to be ready. Check MySQL logs:
```bash
kubectl logs -n kbk-prod deployment/mysql-deployment
```

### Pods not starting
Check PVC status and storage class:
```bash
kubectl get pvc -n kbk-prod
kubectl describe pvc -n kbk-prod
```

