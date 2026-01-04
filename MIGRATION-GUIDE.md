# Quick Migration Guide - Preserve Player Data

## 🎯 Goal
Migrate to persistent storage WITHOUT losing any player data.

## ⚡ Quick Steps (5 minutes)

### Step 1: Backup Current Data

```bash
# Backup production data
./scripts/backup-player-data.sh kbk-prod

# Backup staging (optional)
./scripts/backup-player-data.sh kbk-staging

# Backup dev (optional)
./scripts/backup-player-data.sh kbk-dev
```

This creates backups in `./backups/YYYYMMDD-HHMMSS/`

### Step 2: Deploy New Configuration

```bash
# Commit and push the changes
git add k8s/base/kbk-deployment.yaml k8s/base/kbk-pvc.yaml
git commit -m "fix: add persistent storage for player data"
git push

# ArgoCD will auto-sync, or force it:
argocd app sync kbk-prod
argocd app sync kbk-staging
argocd app sync kbk-dev
```

### Step 3: Wait for Deployment

```bash
# Wait for production to be ready
kubectl wait -n kbk-prod --for=condition=ready pod -l app=kbk --timeout=300s

# Check the init container logs
kubectl logs -n kbk-prod -l app=kbk -c init-kbk-data
```

### Step 4: Restore Data (If Needed)

If you had existing players, restore the backup:

```bash
# Find your backup directory
ls -la backups/

# Restore to production
./scripts/restore-player-data.sh backups/20240116-123456 kbk-prod

# Restart to pick up restored data
kubectl rollout restart -n kbk-prod deployment/kbk-deployment
kubectl wait -n kbk-prod --for=condition=ready pod -l app=kbk --timeout=300s
```

### Step 5: Verify

```bash
# Connect to the MUD
telnet 192.168.0.20 8989

# Log in with an existing character
# Verify everything looks correct

# Make a change and save
save

# Test persistence by restarting
kubectl rollout restart -n kbk-prod deployment/kbk-deployment
kubectl wait -n kbk-prod --for=condition=ready pod -l app=kbk --timeout=300s

# Reconnect and verify the change persisted
telnet 192.168.0.20 8989
```

## ✅ Success Indicators

- [ ] Backups created successfully
- [ ] New deployment running
- [ ] Init container shows "Initialization complete!"
- [ ] Three new PVCs created (player, sys, area)
- [ ] Existing players can log in
- [ ] Changes persist across pod restarts

## 🔍 Troubleshooting

### "No KBK pod found"
The deployment might not be running yet. Wait a minute and try again.

### "Initialization complete!" but no player files
The init container only copies if volumes are empty. If you need to restore data, use the restore script.

### Players can't log in after migration
Make sure you ran the restore script and restarted the deployment.

### PVCs stuck in "Pending"
Check your storage class:
```bash
kubectl get storageclass
kubectl describe pvc -n kbk-prod
```

## 📋 What Changed

**Before:**
- Player files baked into Docker image
- Lost on every deployment
- No persistence

**After:**
- Player files on persistent volumes
- Survive deployments and restarts
- Automatic initialization for fresh installs

## 🎉 Done!

Your player data is now safe! Future deployments will preserve all player characters, system files, and area modifications.

For more details, see [PERSISTENT-STORAGE-FIX.md](PERSISTENT-STORAGE-FIX.md)

