#!/bin/bash
# Restore player data to KBK deployment
# Usage: ./scripts/restore-player-data.sh <backup_dir> [namespace]

set -e

BACKUP_DIR=$1
NAMESPACE=${2:-kbk-prod}

if [ -z "$BACKUP_DIR" ]; then
    echo "❌ Error: Backup directory required"
    echo "Usage: $0 <backup_dir> [namespace]"
    echo "Example: $0 ./backups/20240116-123456 kbk-prod"
    exit 1
fi

if [ ! -d "$BACKUP_DIR" ]; then
    echo "❌ Error: Backup directory not found: $BACKUP_DIR"
    exit 1
fi

echo "🔍 Restoring KBK data to namespace: $NAMESPACE"
echo "📁 From backup: $BACKUP_DIR"

# Get the pod name
POD_NAME=$(kubectl get pod -n "$NAMESPACE" -l app=kbk -o jsonpath='{.items[0].metadata.name}')

if [ -z "$POD_NAME" ]; then
    echo "❌ Error: No KBK pod found in namespace $NAMESPACE"
    echo "Make sure the deployment is running first."
    exit 1
fi

echo "📦 Found pod: $POD_NAME"

# Restore player data
if [ -f "$BACKUP_DIR/player-backup.tar.gz" ]; then
    echo "💾 Restoring player data..."
    kubectl cp "$BACKUP_DIR/player-backup.tar.gz" "$NAMESPACE/$POD_NAME:/tmp/player-backup.tar.gz"
    kubectl exec -n "$NAMESPACE" "$POD_NAME" -- tar xzf /tmp/player-backup.tar.gz -C /kbk
    kubectl exec -n "$NAMESPACE" "$POD_NAME" -- rm /tmp/player-backup.tar.gz
    echo "   ✅ Player data restored"
else
    echo "   ⚠️  No player backup found, skipping"
fi

# Restore sys data
if [ -f "$BACKUP_DIR/sys-backup.tar.gz" ]; then
    echo "💾 Restoring sys data..."
    kubectl cp "$BACKUP_DIR/sys-backup.tar.gz" "$NAMESPACE/$POD_NAME:/tmp/sys-backup.tar.gz"
    kubectl exec -n "$NAMESPACE" "$POD_NAME" -- tar xzf /tmp/sys-backup.tar.gz -C /kbk
    kubectl exec -n "$NAMESPACE" "$POD_NAME" -- rm /tmp/sys-backup.tar.gz
    echo "   ✅ Sys data restored"
else
    echo "   ⚠️  No sys backup found, skipping"
fi

# Restore area data
if [ -f "$BACKUP_DIR/area-backup.tar.gz" ]; then
    echo "💾 Restoring area data..."
    kubectl cp "$BACKUP_DIR/area-backup.tar.gz" "$NAMESPACE/$POD_NAME:/tmp/area-backup.tar.gz"
    kubectl exec -n "$NAMESPACE" "$POD_NAME" -- tar xzf /tmp/area-backup.tar.gz -C /kbk
    kubectl exec -n "$NAMESPACE" "$POD_NAME" -- rm /tmp/area-backup.tar.gz
    echo "   ✅ Area data restored"
else
    echo "   ⚠️  No area backup found, skipping"
fi

echo ""
echo "✅ Restore complete!"
echo ""
echo "⚠️  IMPORTANT: Restart the deployment to pick up the restored data:"
echo "  kubectl rollout restart -n $NAMESPACE deployment/kbk-deployment"
echo "  kubectl wait -n $NAMESPACE --for=condition=ready pod -l app=kbk --timeout=300s"

