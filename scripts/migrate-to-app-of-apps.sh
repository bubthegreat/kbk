#!/bin/bash
set -e

echo "=========================================="
echo "KBK Migration to App of Apps"
echo "=========================================="
echo ""
echo "This script will:"
echo "1. Delete existing manual deployments"
echo "2. Deploy the App of Apps to ArgoCD"
echo "3. Verify ArgoCD takes over management"
echo ""
read -p "Continue? (y/n) " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Aborted."
    exit 1
fi

echo ""
echo "Step 1: Checking current deployments..."
echo "=========================================="
kubectl get all -n kbk-prod 2>/dev/null || echo "kbk-prod namespace not found"
kubectl get all -n kbk-staging 2>/dev/null || echo "kbk-staging namespace not found"
kubectl get all -n kbk-dev 2>/dev/null || echo "kbk-dev namespace not found"

echo ""
echo "Step 2: Deleting existing manual deployments..."
echo "=========================================="

# Delete prod
if kubectl get namespace kbk-prod &>/dev/null; then
    echo "Deleting kbk-prod resources..."
    kubectl delete -k k8s/overlays/prod --ignore-not-found=true
    echo "✓ kbk-prod deleted"
else
    echo "✓ kbk-prod namespace doesn't exist"
fi

# Delete staging
if kubectl get namespace kbk-staging &>/dev/null; then
    echo "Deleting kbk-staging resources..."
    kubectl delete -k k8s/overlays/kbk-staging --ignore-not-found=true
    echo "✓ kbk-staging deleted"
else
    echo "✓ kbk-staging namespace doesn't exist"
fi

# Delete dev
if kubectl get namespace kbk-dev &>/dev/null; then
    echo "Deleting kbk-dev resources..."
    kubectl delete -k k8s/overlays/dev --ignore-not-found=true
    echo "✓ kbk-dev deleted"
else
    echo "✓ kbk-dev namespace doesn't exist"
fi

# Delete TCP ConfigMap (will be recreated by ArgoCD)
echo "Deleting ingress-nginx TCP ConfigMap..."
kubectl delete -f k8s/ingress-nginx/tcp-services-configmap.yaml --ignore-not-found=true
echo "✓ TCP ConfigMap deleted"

echo ""
echo "Step 3: Waiting for resources to be fully deleted..."
echo "=========================================="
sleep 5

# Wait for namespaces to be deleted (if they are being deleted)
for ns in kbk-prod kbk-staging kbk-dev; do
    if kubectl get namespace $ns &>/dev/null; then
        echo "Waiting for namespace $ns to be deleted..."
        kubectl wait --for=delete namespace/$ns --timeout=60s 2>/dev/null || true
    fi
done

echo ""
echo "Step 4: Deploying App of Apps..."
echo "=========================================="
kubectl apply -f argocd/kbk-app-of-apps.yaml
echo "✓ App of Apps deployed"

echo ""
echo "Step 5: Waiting for ArgoCD to create applications..."
echo "=========================================="
sleep 10

echo ""
echo "Step 6: Checking ArgoCD applications..."
echo "=========================================="
kubectl get applications -n argocd | grep kbk || echo "No KBK applications found yet"

echo ""
echo "Step 7: Waiting for applications to sync..."
echo "=========================================="
echo "This may take a few minutes..."
sleep 30

echo ""
echo "Step 8: Final status check..."
echo "=========================================="
echo ""
echo "ArgoCD Applications:"
kubectl get applications -n argocd | grep kbk
echo ""
echo "Pods in kbk-prod:"
kubectl get pods -n kbk-prod 2>/dev/null || echo "Namespace not ready yet"
echo ""
echo "Pods in kbk-staging:"
kubectl get pods -n kbk-staging 2>/dev/null || echo "Namespace not ready yet"
echo ""
echo "Pods in kbk-dev:"
kubectl get pods -n kbk-dev 2>/dev/null || echo "Namespace not ready yet"

echo ""
echo "=========================================="
echo "Migration Complete!"
echo "=========================================="
echo ""
echo "Next steps:"
echo "1. Monitor ArgoCD UI or run: argocd app list | grep kbk"
echo "2. Wait for all apps to show 'Synced' and 'Healthy'"
echo "3. Test connections:"
echo "   - telnet 192.168.0.20 8989  (prod)"
echo "   - telnet 192.168.0.20 8988  (staging)"
echo "   - telnet 192.168.0.20 8987  (dev)"
echo ""
echo "To check sync status:"
echo "  argocd app get kbk-prod"
echo "  argocd app get kbk-staging"
echo "  argocd app get kbk-dev"
echo ""

