#!/bin/bash
# Backup player data from running KBK deployment
# Usage: ./scripts/backup-player-data.sh [namespace]

set -e

NAMESPACE=${1:-kbk-prod}
BACKUP_DIR="./backups/$(date +%Y%m%d-%H%M%S)"

echo "🔍 Backing up KBK data from namespace: $NAMESPACE"
echo "📁 Backup directory: $BACKUP_DIR"

# Create backup directory
mkdir -p "$BACKUP_DIR"

# Get the pod name
POD_NAME=$(kubectl get pod -n "$NAMESPACE" -l app=kbk -o jsonpath='{.items[0].metadata.name}')

if [ -z "$POD_NAME" ]; then
    echo "❌ Error: No KBK pod found in namespace $NAMESPACE"
    exit 1
fi

echo "📦 Found pod: $POD_NAME"

# Backup player data
echo "💾 Backing up player data..."
kubectl exec -n "$NAMESPACE" "$POD_NAME" -- tar czf /tmp/player-backup.tar.gz -C /kbk player 2>/dev/null || true
kubectl cp "$NAMESPACE/$POD_NAME:/tmp/player-backup.tar.gz" "$BACKUP_DIR/player-backup.tar.gz"
kubectl exec -n "$NAMESPACE" "$POD_NAME" -- rm /tmp/player-backup.tar.gz

# Backup sys data
echo "💾 Backing up sys data..."
kubectl exec -n "$NAMESPACE" "$POD_NAME" -- tar czf /tmp/sys-backup.tar.gz -C /kbk sys 2>/dev/null || true
kubectl cp "$NAMESPACE/$POD_NAME:/tmp/sys-backup.tar.gz" "$BACKUP_DIR/sys-backup.tar.gz"
kubectl exec -n "$NAMESPACE" "$POD_NAME" -- rm /tmp/sys-backup.tar.gz

# Backup area data
echo "💾 Backing up area data..."
kubectl exec -n "$NAMESPACE" "$POD_NAME" -- tar czf /tmp/area-backup.tar.gz -C /kbk area 2>/dev/null || true
kubectl cp "$NAMESPACE/$POD_NAME:/tmp/area-backup.tar.gz" "$BACKUP_DIR/area-backup.tar.gz"
kubectl exec -n "$NAMESPACE" "$POD_NAME" -- rm /tmp/area-backup.tar.gz

# List player files
echo ""
echo "📋 Player files backed up:"
tar tzf "$BACKUP_DIR/player-backup.tar.gz" | grep "\.plr$" | head -20
PLAYER_COUNT=$(tar tzf "$BACKUP_DIR/player-backup.tar.gz" | grep -c "\.plr$" || echo "0")
echo "   ... Total: $PLAYER_COUNT player files"

echo ""
echo "✅ Backup complete!"
echo "📁 Backup location: $BACKUP_DIR"
echo ""
echo "To restore this backup to a new deployment:"
echo "  ./scripts/restore-player-data.sh $BACKUP_DIR $NAMESPACE"

