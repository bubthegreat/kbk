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

1. **Kubernetes cluster** (homelab-k3s)
2. **ArgoCD installed** in the cluster
3. **ingress-nginx controller** deployed
4. **MetalLB** for LoadBalancer services
5. **Storage class** (Longhorn or similar)
6. **GitHub repository** at https://github.com/bubthegreat/kbk.git

### Deployment Method 1: App of Apps (Recommended - GitOps)

This is the **recommended approach** for production deployments. One command deploys everything!

```bash
# Step 1: Push your code to GitHub
git remote set-url origin https://github.com/bubthegreat/kbk.git
git push -u origin master

# Step 2: Deploy the App of Apps to ArgoCD
kubectl apply -f argocd/kbk-app-of-apps.yaml

# Step 3: Watch ArgoCD create all applications
kubectl get applications -n argocd -w
```

**What happens automatically:**
1. ArgoCD creates the `kbk-app-of-apps` Application
2. The App of Apps creates:
   - `kbk-environments` ApplicationSet
   - `kbk-ingress-nginx-tcp-config` Application
3. The ApplicationSet creates 3 environment Applications:
   - `kbk-prod` → deploys to `kbk-prod` namespace
   - `kbk-staging` → deploys to `kbk-staging` namespace
   - `kbk-dev` → deploys to `kbk-dev` namespace
4. All applications auto-sync from GitHub
5. All namespaces, deployments, services, and ingresses are created

**Verify deployment:**
```bash
# Check all KBK applications
kubectl get applications -n argocd | grep kbk

# Expected output:
# kbk-app-of-apps                  Synced   Healthy
# kbk-environments                 Synced   Healthy
# kbk-prod                         Synced   Healthy
# kbk-staging                      Synced   Healthy
# kbk-dev                          Synced   Healthy
# kbk-ingress-nginx-tcp-config     Synced   Healthy

# Check pods in all environments
kubectl get pods -n kbk-prod
kubectl get pods -n kbk-staging
kubectl get pods -n kbk-dev
```

**Benefits of this approach:**
- ✅ Single command deployment
- ✅ GitOps - all changes tracked in Git
- ✅ Auto-sync - push to GitHub, ArgoCD deploys
- ✅ Self-healing - manual changes are reverted
- ✅ Easy rollback - revert Git commit
- ✅ Audit trail - all changes in Git history

### Deployment Method 2: Manual kubectl (For Testing/Development)

Use this for quick testing or when ArgoCD is not available.

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

**Note:** With manual deployment, you need to manually apply changes each time you update the manifests.

## GitOps Workflow (App of Apps)

When using the App of Apps pattern, your workflow is:

```bash
# 1. Make changes locally
vim k8s/overlays/prod/kustomization.yaml
vim src/some-code.c

# 2. Test locally (optional)
kubectl apply -k k8s/overlays/dev --dry-run=client

# 3. Commit and push to GitHub
git add .
git commit -m "Update production settings"
git push

# 4. ArgoCD automatically syncs (within ~3 minutes)
# Watch the sync happen:
argocd app get kbk-prod

# 5. Verify deployment
kubectl get pods -n kbk-prod
telnet 192.168.0.20 8989
```

**That's it!** No manual kubectl applies needed. Everything is managed through Git.

## Environments

| Environment | Namespace | Telnet | HTTP | ArgoCD App |
|-------------|-----------|--------|------|------------|
| Production | `kbk-prod` | `telnet 192.168.0.20 8989` | `http://kbk.bubtaylor.com` | `kbk-prod` |
| Staging | `kbk-staging` | `telnet 192.168.0.20 8988` | `http://kbk-staging.bubtaylor.com` | `kbk-staging` |
| Development | `kbk-dev` | `telnet 192.168.0.20 8987` | `http://kbk-dev.bubtaylor.com` | `kbk-dev` |

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

**With App of Apps (GitOps):**
```bash
# 1. Make your changes to the code/manifests
vim k8s/overlays/prod/kustomization.yaml

# 2. Commit and push to GitHub
git add .
git commit -m "Update production configuration"
git push

# 3. ArgoCD auto-syncs within ~3 minutes
# Or force immediate sync:
argocd app sync kbk-prod

# Or sync all KBK apps:
argocd app sync -l app=kbk
```

**With manual deployment:**
```bash
# Apply changes directly
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

**App of Apps not creating child apps:**
```bash
# Check App of Apps status
kubectl get application -n argocd kbk-app-of-apps -o yaml

# Check if it can reach GitHub
argocd app get kbk-app-of-apps

# Force refresh
argocd app get kbk-app-of-apps --refresh --hard-refresh

# Check for errors
kubectl logs -n argocd deployment/argocd-application-controller | grep kbk
```

**Child application sync issues:**
```bash
# Check application status
argocd app get kbk-prod

# View what's different between Git and cluster
argocd app diff kbk-prod

# Force sync (overrides any manual changes)
argocd app sync kbk-prod --force

# Refresh from Git (re-fetch latest)
argocd app get kbk-prod --refresh

# Sync all KBK apps at once
argocd app sync -l app=kbk
```

**Application stuck in "Progressing":**
```bash
# Check the actual resources
kubectl get all -n kbk-prod

# Check events
kubectl get events -n kbk-prod --sort-by='.lastTimestamp'

# Check ArgoCD application events
kubectl describe application -n argocd kbk-prod
```

## Building and Pushing Images

```bash
# Build new image
docker build -t your-registry/kbk:latest .

# Push to registry
docker push your-registry/kbk:latest

# Update image in manifests
vim k8s/base/kbk-deployment.yaml  # Update image tag

# With App of Apps - commit and push
git add k8s/base/kbk-deployment.yaml
git commit -m "Update KBK image to latest"
git push
# ArgoCD will auto-deploy the new image

# With manual deployment
kubectl apply -k k8s/overlays/prod
kubectl rollout restart deployment -n kbk-prod kbk-deployment
```

See the [Makefile](Makefile) for available build targets.

## Cleanup / Removal

### Remove everything (App of Apps)

```bash
# Delete the App of Apps (removes all child apps and resources)
kubectl delete application -n argocd kbk-app-of-apps

# This will cascade delete:
# - kbk-environments (ApplicationSet)
# - kbk-prod, kbk-staging, kbk-dev (Applications)
# - kbk-ingress-nginx-tcp-config (Application)
# - All deployments, services, PVCs in kbk-* namespaces

# Optionally, delete the namespaces (if not auto-deleted)
kubectl delete namespace kbk-prod kbk-staging kbk-dev
```

### Remove a single environment

```bash
# Delete just one environment
kubectl delete application -n argocd kbk-prod

# Or remove it from the ApplicationSet
# Edit argocd/kbk-applicationset.yaml and remove the prod element
# Then commit and push - ArgoCD will remove it
```

### Remove with manual deployment

```bash
# Delete each environment
kubectl delete -k k8s/overlays/prod
kubectl delete -k k8s/overlays/kbk-staging
kubectl delete -k k8s/overlays/dev

# Delete TCP config
kubectl delete -f k8s/ingress-nginx/tcp-services-configmap.yaml

# Delete namespaces
kubectl delete namespace kbk-prod kbk-staging kbk-dev
```

## Documentation

- [ArgoCD Setup](argocd/README.md) - Detailed ArgoCD configuration and App of Apps
- [Kubernetes Deployment](k8s/README.md) - Kubernetes manifest details
- [Main README](README.md) - Project overview and local development

## Support

For issues or questions:
1. Check the troubleshooting sections above
2. Review logs: `kubectl logs -n <namespace> deployment/<deployment-name>`
3. Check ArgoCD UI for sync status and application health
4. Use `argocd app get <app-name>` for detailed application status
5. Review the detailed READMEs in `argocd/` and `k8s/` directories

## Quick Reference Commands

### App of Apps Management
```bash
# Deploy everything
kubectl apply -f argocd/kbk-app-of-apps.yaml

# Check status
kubectl get applications -n argocd | grep kbk
argocd app list | grep kbk

# Sync all
argocd app sync -l app=kbk

# Remove everything
kubectl delete application -n argocd kbk-app-of-apps
```

### Environment Access
```bash
# Production
telnet 192.168.0.20 8989
curl http://kbk.bubtaylor.com

# Staging
telnet 192.168.0.20 8988
curl http://kbk-staging.bubtaylor.com

# Development
telnet 192.168.0.20 8987
curl http://kbk-dev.bubtaylor.com
```

