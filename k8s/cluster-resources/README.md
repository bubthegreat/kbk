# Cluster-Wide Resources

This directory contains Kubernetes resources that are **cluster-wide** (not namespaced) and shared across all KBK environments.

## Resources

### StorageClass: longhorn-retain

**File**: `storageclass-retain.yaml`

A custom StorageClass with `Retain` reclaim policy to protect PersistentVolumes from deletion.

**Key Features**:
- `reclaimPolicy: Retain` - PVs are not deleted when PVCs are removed
- Based on Longhorn storage provisioner
- Used by all KBK environments (prod, staging, dev)

**Usage**:
All KBK PVCs reference this StorageClass:
```yaml
spec:
  storageClassName: longhorn-retain
```

## Deployment

### Via ArgoCD (Recommended)

This directory is managed by the `kbk-cluster-resources` ArgoCD Application:

```bash
# Deploy all ArgoCD applications (includes cluster resources)
kubectl apply -k argocd/

# Or deploy just cluster resources
kubectl apply -f argocd/cluster-resources.yaml
```

### Manual Deployment

```bash
# Apply cluster resources directly
kubectl apply -k k8s/cluster-resources/
```

## Why Separate?

Cluster-wide resources like StorageClass are **not namespaced** and should only be created once per cluster, not per environment.

By separating them into their own ArgoCD Application, we:
- ✅ Avoid "SharedResourceWarning" from multiple ArgoCD apps managing the same resource
- ✅ Keep cluster-wide config separate from environment-specific config
- ✅ Follow the same pattern as `ingress-nginx-tcp-config`

## Adding New Cluster Resources

If you need to add more cluster-wide resources (e.g., ClusterRole, ClusterRoleBinding, CustomResourceDefinitions), add them here:

1. Create the YAML file in this directory
2. Add it to `kustomization.yaml`
3. Commit and push - ArgoCD will sync automatically

