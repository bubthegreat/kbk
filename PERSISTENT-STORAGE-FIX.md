# Persistent Storage Fix - Player Data Preservation

## 🚨 Problem

Previously, every deployment was **wiping out all player data** because:

1. Player files were baked into the Docker image
2. The persistent volume existed but wasn't mounted to `/kbk/player`
3. Each new pod would start with the player files from the Docker image build time

**Result**: Players lost their characters on every deployment! 😱

## ✅ Solution

The deployment now uses **separate persistent volumes** for all critical data:

| Directory | Purpose | PVC Name | Size |
|-----------|---------|----------|------|
| `/kbk/player/` | Player character files (`.plr`) | `kbk-player-pvc` | 1Gi |
| `/kbk/sys/` | System files (bugs, bans, bounties, etc.) | `kbk-sys-pvc` | 1Gi |
| `/kbk/area/` | Area files + compiled binary | `kbk-area-pvc` | 2Gi |

### How It Works

1. **Init Container** runs first on pod startup
   - Checks if persistent volumes are empty
   - If empty, copies initial data from Docker image
   - If not empty, skips (preserves existing data)

2. **Main Container** mounts the persistent volumes
   - All changes to player files are saved to PVC
   - Survives pod restarts, deployments, and updates
   - Data persists until you explicitly delete the PVCs

## 🔄 Migration Steps

### For Fresh Deployments (No Existing Data)

Just deploy normally - the init container will set everything up:

```bash
git pull
git add k8s/base/
git commit -m "fix: add persistent storage for player data"
git push

# ArgoCD will sync automatically, or force it:
argocd app sync kbk-prod
```

### For Existing Deployments (With Player Data to Preserve)

⚠️ **IMPORTANT**: If you have existing player data you want to keep, follow these steps:

#### Option 1: Backup and Restore (Recommended)

```bash
# 1. Backup existing player data from running pod
kubectl exec -n kbk-prod deployment/kbk-deployment -- tar czf /tmp/player-backup.tar.gz -C /kbk player
kubectl cp kbk-prod/$(kubectl get pod -n kbk-prod -l app=kbk -o jsonpath='{.items[0].metadata.name}'):/tmp/player-backup.tar.gz ./player-backup.tar.gz

# 2. Backup sys data
kubectl exec -n kbk-prod deployment/kbk-deployment -- tar czf /tmp/sys-backup.tar.gz -C /kbk sys
kubectl cp kbk-prod/$(kubectl get pod -n kbk-prod -l app=kbk -o jsonpath='{.items[0].metadata.name}'):/tmp/sys-backup.tar.gz ./sys-backup.tar.gz

# 3. Backup area data
kubectl exec -n kbk-prod deployment/kbk-deployment -- tar czf /tmp/area-backup.tar.gz -C /kbk area
kubectl cp kbk-prod/$(kubectl get pod -n kbk-prod -l app=kbk -o jsonpath='{.items[0].metadata.name}'):/tmp/area-backup.tar.gz ./area-backup.tar.gz

# 4. Deploy the new configuration
git pull
argocd app sync kbk-prod

# 5. Wait for new pod to be ready
kubectl wait -n kbk-prod --for=condition=ready pod -l app=kbk --timeout=300s

# 6. Restore the data to persistent volumes
NEW_POD=$(kubectl get pod -n kbk-prod -l app=kbk -o jsonpath='{.items[0].metadata.name}')

kubectl cp ./player-backup.tar.gz kbk-prod/$NEW_POD:/tmp/player-backup.tar.gz
kubectl exec -n kbk-prod $NEW_POD -- tar xzf /tmp/player-backup.tar.gz -C /kbk

kubectl cp ./sys-backup.tar.gz kbk-prod/$NEW_POD:/tmp/sys-backup.tar.gz
kubectl exec -n kbk-prod $NEW_POD -- tar xzf /tmp/sys-backup.tar.gz -C /kbk

kubectl cp ./area-backup.tar.gz kbk-prod/$NEW_POD:/tmp/area-backup.tar.gz
kubectl exec -n kbk-prod $NEW_POD -- tar xzf /tmp/area-backup.tar.gz -C /kbk

# 7. Restart the pod to pick up the restored data
kubectl rollout restart -n kbk-prod deployment/kbk-deployment
```

#### Option 2: Direct Volume Migration (Advanced)

If you're comfortable with Kubernetes, you can:

1. Scale down the deployment
2. Create a migration pod that mounts both old and new PVCs
3. Copy data between them
4. Scale up with new configuration

## 🧪 Testing

After migration, verify player data is preserved:

```bash
# Connect to the MUD
telnet 192.168.0.20 8989

# Log in with an existing character
# Make a change (e.g., move to a different room, change equipment)
# Type 'save' to save your character

# Trigger a deployment to test persistence
kubectl rollout restart -n kbk-prod deployment/kbk-deployment

# Wait for new pod
kubectl wait -n kbk-prod --for=condition=ready pod -l app=kbk --timeout=300s

# Reconnect and verify your character still has the changes
telnet 192.168.0.20 8989
```

## 📊 What's Persisted Now

### Player Directory (`/kbk/player/`)
- `*.plr` - Player character files
- `Player.lst` - List of all players

### Sys Directory (`/kbk/sys/`)
- `bugs.txt` - Bug reports
- `typos.txt` - Typo reports
- `ban.txt` - Banned sites
- `bounties.txt` - Bounty information
- `debug.txt` - Debug logs
- `crash.txt` - Crash logs
- `citems.txt` - Cabal items
- `glog.txt` - God logs
- `login.txt` - Login screen
- `itemfix.txt` - Item fixes
- `*.not` - Note files (changes, ideas, news, penalty)

### Area Directory (`/kbk/area/`)
- `*.are` - Area files (can be modified via OLC)
- `area.lst` - Area list
- `pos2` - Compiled MUD binary
- `start-kbk.sh` - Startup script

### What's NOT Persisted (By Design)
- `/kbk/localxfer/` - Uses emptyDir (temporary transfer directory)
- Notes - Stored in MySQL database (already persistent)

## 🔍 Verification Commands

```bash
# Check PVCs are created
kubectl get pvc -n kbk-prod

# Expected output:
# NAME              STATUS   VOLUME                                     CAPACITY
# kbk-player-pvc    Bound    pvc-xxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx     1Gi
# kbk-sys-pvc       Bound    pvc-xxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx     1Gi
# kbk-area-pvc      Bound    pvc-xxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx     2Gi
# mysql-pvc         Bound    pvc-xxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx     10Gi

# Check volumes are mounted
kubectl exec -n kbk-prod deployment/kbk-deployment -- df -h | grep kbk

# List player files
kubectl exec -n kbk-prod deployment/kbk-deployment -- ls -la /kbk/player/

# Check init container ran successfully
kubectl logs -n kbk-prod -l app=kbk -c init-kbk-data
```

## 🎯 Benefits

- ✅ **Player data survives deployments** - No more lost characters!
- ✅ **OLC changes persist** - Area modifications saved across updates
- ✅ **System files preserved** - Bans, bugs, bounties all kept
- ✅ **Automatic initialization** - Fresh deployments get starter data
- ✅ **Safe updates** - Code changes don't affect player data

## ⚠️ Important Notes

1. **PVC Deletion**: If you delete the PVCs, you'll lose all player data. Be careful!
2. **Backup Strategy**: Consider setting up automated backups of the PVCs
3. **Storage Class**: Make sure your cluster has a default storage class (Longhorn, etc.)
4. **Multi-Environment**: Each environment (prod, staging, dev) has its own PVCs

## 🗑️ Cleanup Old PVC (If Exists)

If you had the old `kbk-pvc` that wasn't being used:

```bash
# Check if it exists
kubectl get pvc -n kbk-prod kbk-pvc

# If it exists and is empty, you can delete it
kubectl delete pvc -n kbk-prod kbk-pvc
```

