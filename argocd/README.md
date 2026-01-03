# ArgoCD Configuration for KBK

This directory contains ArgoCD ApplicationSet and Application manifests for automated deployment of KBK environments.

## Files

- `kbk-applicationset.yaml` - ApplicationSet that deploys all KBK environments (prod, staging, dev)
- `ingress-nginx-tcp-config.yaml` - Application for managing the ingress-nginx TCP ConfigMap

## Prerequisites

1. **ArgoCD installed** in your cluster
2. **GitHub repository** at `https://github.com/bubthegreat/kbk.git`
3. **ingress-nginx controller** already installed and configured
4. **ArgoCD CLI** (optional, for testing)

## Deployment

### Option 1: Apply directly to cluster

```bash
# Apply the ApplicationSet (creates apps for all environments)
kubectl apply -f argocd/kbk-applicationset.yaml

# Apply the ingress-nginx TCP config
kubectl apply -f argocd/ingress-nginx-tcp-config.yaml
```

### Option 2: Using ArgoCD CLI

```bash
# Create the ApplicationSet
argocd appset create argocd/kbk-applicationset.yaml

# Create the ingress-nginx config app
argocd app create -f argocd/ingress-nginx-tcp-config.yaml
```

### Option 3: Via ArgoCD UI

1. Navigate to ArgoCD UI
2. Click "New App" → "Edit as YAML"
3. Paste the contents of `kbk-applicationset.yaml`
4. Click "Create"
5. Repeat for `ingress-nginx-tcp-config.yaml`

## What Gets Deployed

The ApplicationSet will automatically create and manage 3 ArgoCD Applications:

### 1. kbk-prod
- **Namespace**: `kbk-prod`
- **Source**: `k8s/overlays/prod`
- **Telnet Port**: 8989
- **HTTP**: `kbk.bubtaylor.com`

### 2. kbk-staging
- **Namespace**: `kbk-staging`
- **Source**: `k8s/overlays/kbk-staging`
- **Telnet Port**: 8988
- **HTTP**: `kbk-staging.bubtaylor.com`

### 3. kbk-dev
- **Namespace**: `kbk-dev`
- **Source**: `k8s/overlays/dev`
- **Telnet Port**: 8987
- **HTTP**: `kbk-dev.bubtaylor.com`

## Sync Policy

All applications are configured with:
- **Automated sync**: Changes in Git are automatically deployed
- **Self-heal**: Manual changes to cluster are reverted
- **Prune**: Deleted resources in Git are removed from cluster
- **Auto-create namespaces**: Namespaces are created automatically

## Monitoring

### Check ApplicationSet status
```bash
kubectl get applicationset -n argocd kbk-environments
```

### Check generated Applications
```bash
kubectl get applications -n argocd | grep kbk
```

### View Application details
```bash
# Via kubectl
kubectl get application -n argocd kbk-prod -o yaml

# Via ArgoCD CLI
argocd app get kbk-prod
argocd app get kbk-staging
argocd app get kbk-dev
```

### View sync status
```bash
argocd app list | grep kbk
```

## Manual Sync

If you need to manually trigger a sync:

```bash
# Sync a specific environment
argocd app sync kbk-prod
argocd app sync kbk-staging
argocd app sync kbk-dev

# Sync all KBK apps
argocd app sync -l app=kbk
```

## Troubleshooting

### Application stuck in "OutOfSync"
```bash
# Check the diff
argocd app diff kbk-prod

# Force sync
argocd app sync kbk-prod --force
```

### Application health degraded
```bash
# Check application details
argocd app get kbk-prod

# Check pod status
kubectl get pods -n kbk-prod
```

### Changes not being picked up
```bash
# Refresh the application (re-fetch from Git)
argocd app get kbk-prod --refresh

# Hard refresh (ignore cache)
argocd app get kbk-prod --hard-refresh
```

## Important Notes

### ingress-nginx TCP ConfigMap
The `ingress-nginx-tcp-config.yaml` Application manages the TCP services ConfigMap that routes telnet traffic. This is separate from the environment applications because:
1. It lives in the `ingress-nginx` namespace
2. It affects all environments
3. Changes require ingress-nginx controller restart

After applying changes to the TCP ConfigMap, you may need to restart the ingress-nginx controller:
```bash
kubectl rollout restart deployment -n ingress-nginx ingressnginx-ingress-nginx-controller
```

### Repository Migration
When you move from GitLab to GitHub:
1. Push your code to `https://github.com/bubthegreat/kbk.git`
2. The ApplicationSet will automatically start syncing from GitHub
3. No changes needed to the ApplicationSet manifest

### Customizing Environments
To add or modify environments, edit the `generators.list.elements` section in `kbk-applicationset.yaml`:

```yaml
generators:
  - list:
      elements:
        - environment: prod
          namespace: kbk-prod
          overlay: prod
          telnetPort: "8989"
        # Add more environments here
```

## Cleanup

To remove all KBK applications:

```bash
# Delete the ApplicationSet (removes all generated apps)
kubectl delete applicationset -n argocd kbk-environments

# Delete the ingress-nginx config app
kubectl delete application -n argocd kbk-ingress-nginx-tcp-config

# Optionally, delete the namespaces
kubectl delete namespace kbk-prod kbk-staging kbk-dev
```

