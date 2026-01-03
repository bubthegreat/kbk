# CI/CD Pipeline Setup - Quick Start

## 🚀 What Was Added

A complete GitHub Actions CI/CD pipeline that automatically:
1. ✅ Builds Docker images when you push code
2. ✅ Tags images with commit SHA for version tracking
3. ✅ Pushes images to Docker Hub
4. ✅ Updates Kubernetes manifests automatically
5. ✅ Triggers ArgoCD to deploy to all environments

## ⚡ Quick Setup (5 minutes)

### Step 1: Create Docker Hub Token

1. Go to: https://hub.docker.com/settings/security
2. Click **"New Access Token"**
3. Name: `GitHub Actions KBK`
4. Copy the token (save it somewhere - you won't see it again!)

### Step 2: Add Token to GitHub

1. Go to: https://github.com/bubthegreat/kbk/settings/secrets/actions
2. Click **"New repository secret"**
3. Name: `DOCKER_HUB_TOKEN`
4. Value: Paste the token from Step 1
5. Click **"Add secret"**

### Step 3: Enable Workflow Permissions

1. Go to: https://github.com/bubthegreat/kbk/settings/actions
2. Scroll to **"Workflow permissions"**
3. Select **"Read and write permissions"**
4. Check **"Allow GitHub Actions to create and approve pull requests"**
5. Click **"Save"**

### Step 4: Push This Code to GitHub

```bash
# Add the new workflow files
git add .github/workflows/

# Commit
git commit -m "feat: add CI/CD pipeline with automated Docker builds and deployments"

# Push to trigger the first build
git push origin master
```

### Step 5: Watch It Work! 🎉

1. Go to: https://github.com/bubthegreat/kbk/actions
2. Watch your first automated build run
3. After ~5-10 minutes, check your cluster:

```bash
# Pull latest manifest changes
git pull

# Check the updated image tags
cat k8s/base/kbk-deployment.yaml | grep image:

# Watch ArgoCD deploy
argocd app get kbk-prod

# See new pods rolling out
kubectl get pods -n kbk-prod -w
```

## 🎯 How to Use It

### Normal Development Workflow

```bash
# 1. Make your code changes
vim src/act_info.c

# 2. Commit and push
git add src/act_info.c
git commit -m "feat: add new commands feature"
git push

# 3. That's it! The pipeline does the rest:
#    - Builds Docker images
#    - Pushes to Docker Hub
#    - Updates K8s manifests
#    - ArgoCD deploys automatically
```

### Check What's Deployed

```bash
# See current image version in Git
cat k8s/base/kbk-deployment.yaml | grep image:

# See what's actually running
kubectl get deployment -n kbk-prod kbk-deployment -o jsonpath='{.spec.template.spec.containers[0].image}'

# View all available image tags
# https://hub.docker.com/r/bubthegreat/kbk/tags
```

### Manual Trigger

Sometimes you want to rebuild without code changes:

1. Go to: https://github.com/bubthegreat/kbk/actions
2. Click **"Build and Deploy KBK"**
3. Click **"Run workflow"**
4. Click **"Run workflow"** again

## 📦 Image Tagging

Every build creates 3 tags:

| Tag | Example | Purpose |
|-----|---------|---------|
| `latest` | `bubthegreat/kbk:latest` | Always the newest build |
| Short SHA | `bubthegreat/kbk:abc1234` | **Used in K8s manifests** - immutable, traceable |
| Timestamp | `bubthegreat/kbk:20240116-123456-abc1234` | Human-readable, good for debugging |

## 🔄 The Complete Flow

```
You push code
    ↓
GitHub Actions triggers
    ↓
Builds Docker images (KBK + MySQL)
    ↓
Pushes to Docker Hub with 3 tags
    ↓
Updates k8s/base/*.yaml with SHA tag
    ↓
Commits changes back to Git [skip ci]
    ↓
ArgoCD detects Git change (~3 min)
    ↓
Deploys to all environments (prod, staging, dev)
    ↓
Zero-downtime rolling update
    ↓
Done! ✅
```

## 🛠️ Troubleshooting

### "Error: Username and password required"
- You forgot to add the `DOCKER_HUB_TOKEN` secret
- Go back to Step 2 above

### "Permission denied" when committing
- Workflow permissions not enabled
- Go back to Step 3 above

### ArgoCD not deploying
```bash
# Force refresh
argocd app get kbk-prod --refresh --hard-refresh

# Force sync
argocd app sync kbk-prod
```

### Want to skip CI/CD for a commit?
```bash
git commit -m "docs: update README [skip ci]"
```

## 📊 Monitoring

- **Workflow runs**: https://github.com/bubthegreat/kbk/actions
- **Docker images**: https://hub.docker.com/r/bubthegreat/kbk/tags
- **ArgoCD apps**: `argocd app list | grep kbk`

## 🎓 Learn More

See detailed documentation in `.github/workflows/README.md`

## ✅ Verification Checklist

After setup, verify everything works:

- [ ] Docker Hub token added to GitHub secrets
- [ ] Workflow permissions enabled
- [ ] Code pushed to GitHub
- [ ] Workflow ran successfully (check Actions tab)
- [ ] Images pushed to Docker Hub (check hub.docker.com)
- [ ] Manifests updated with new SHA (check git log)
- [ ] ArgoCD synced the changes (check `argocd app get kbk-prod`)
- [ ] New pods running (check `kubectl get pods -n kbk-prod`)

## 🎉 You're Done!

From now on, every time you push code to master:
1. Docker images build automatically
2. Kubernetes manifests update automatically  
3. ArgoCD deploys automatically
4. All environments get the update

**No more manual `docker build` or `docker push` commands needed!**

