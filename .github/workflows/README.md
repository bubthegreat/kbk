# GitHub Actions CI/CD Pipeline

This directory contains the GitHub Actions workflows for building and deploying KBK.

## Workflows

### `build-and-deploy.yml`

Automatically builds Docker images and updates Kubernetes manifests when code changes are pushed.

**Triggers:**
- Push to `master` or `main` branch
- Changes to `src/`, `area/`, `Dockerfile`, `mysql/`, or workflow files
- Manual trigger via GitHub UI

**What it does:**

1. **Build and Push Images**
   - Builds `bubthegreat/kbk` Docker image
   - Builds `bubthegreat/kbk-sql` Docker image
   - Tags images with:
     - `latest` (always updated)
     - Short commit SHA (e.g., `abc1234`)
     - Timestamp + SHA (e.g., `20240116-123456-abc1234`)
   - Pushes all tags to Docker Hub
   - Uses layer caching for faster builds

2. **Update Kubernetes Manifests**
   - Updates `k8s/base/kbk-deployment.yaml` with new image tag
   - Updates `k8s/base/mysql-deployment.yaml` with new image tag
   - Commits changes back to the repository
   - ArgoCD automatically detects and deploys the changes

## Setup Instructions

### 1. Create Docker Hub Access Token

1. Go to https://hub.docker.com/settings/security
2. Click "New Access Token"
3. Name it "GitHub Actions KBK"
4. Copy the token (you won't see it again!)

### 2. Add Secret to GitHub Repository

1. Go to your GitHub repository: https://github.com/bubthegreat/kbk
2. Click **Settings** → **Secrets and variables** → **Actions**
3. Click **New repository secret**
4. Name: `DOCKER_HUB_TOKEN`
5. Value: Paste the Docker Hub token from step 1
6. Click **Add secret**

### 3. Verify Workflow Permissions

1. In your GitHub repository, go to **Settings** → **Actions** → **General**
2. Scroll to **Workflow permissions**
3. Select **Read and write permissions**
4. Check **Allow GitHub Actions to create and approve pull requests**
5. Click **Save**

### 4. Test the Workflow

Push a change to trigger the workflow:

```bash
# Make a small change
echo "# Test CI/CD" >> README.md

# Commit and push
git add README.md
git commit -m "test: trigger CI/CD pipeline"
git push origin master
```

Watch the workflow run:
1. Go to https://github.com/bubthegreat/kbk/actions
2. Click on the latest workflow run
3. Watch the build progress

### 5. Verify Deployment

After the workflow completes:

```bash
# Check that manifests were updated
git pull
cat k8s/base/kbk-deployment.yaml | grep image:

# Watch ArgoCD sync the changes
argocd app get kbk-prod

# Monitor pod rollout
kubectl get pods -n kbk-prod -w
```

## How It Works

### Image Tagging Strategy

Each build creates three tags:

1. **`latest`** - Always points to the most recent build
   - Use for: Development/testing
   - Example: `bubthegreat/kbk:latest`

2. **Short SHA** - Git commit hash (7 characters)
   - Use for: Production deployments (what gets committed to manifests)
   - Example: `bubthegreat/kbk:abc1234`
   - Benefit: Immutable, traceable to exact code version

3. **Timestamp + SHA** - Human-readable timestamp
   - Use for: Debugging, rollbacks
   - Example: `bubthegreat/kbk:20240116-123456-abc1234`
   - Benefit: Easy to see when it was built

### GitOps Flow

```
Code Change → GitHub Push → GitHub Actions → Docker Hub → Git Commit → ArgoCD → Kubernetes
     ↓              ↓              ↓              ↓            ↓           ↓          ↓
  src/*.c      Workflow      Build Images    Push Tags   Update K8s   Detect    Deploy
                Triggers                                  Manifests    Change    Pods
```

### Automatic Rollout

When the workflow updates the manifests:

1. **Commit is pushed** with message like: `chore: update image tags to abc1234 [skip ci]`
   - `[skip ci]` prevents infinite loop of builds

2. **ArgoCD detects change** within ~3 minutes (or immediately if you sync manually)

3. **Kubernetes rolls out** new pods with the updated image
   - Old pods stay running until new ones are healthy
   - Zero-downtime deployment

4. **All environments update** (prod, staging, dev) because they all use the base manifests

## Manual Operations

### Manually Trigger Workflow

1. Go to https://github.com/bubthegreat/kbk/actions
2. Click **Build and Deploy KBK** workflow
3. Click **Run workflow**
4. Select branch and click **Run workflow**

### Build Specific Version

To deploy a specific commit:

```bash
# Find the commit SHA you want
git log --oneline

# Check if image exists for that commit
docker pull bubthegreat/kbk:abc1234

# Manually update manifest
sed -i 's|image: bubthegreat/kbk:.*|image: bubthegreat/kbk:abc1234|g' k8s/base/kbk-deployment.yaml

# Commit and push
git add k8s/base/kbk-deployment.yaml
git commit -m "chore: rollback to abc1234"
git push
```

### Skip CI/CD

To push code changes without triggering the workflow:

```bash
git commit -m "docs: update README [skip ci]"
git push
```

## Troubleshooting

### Workflow Fails at "Log in to Docker Hub"

**Problem**: `Error: Username and password required`

**Solution**: Make sure you added the `DOCKER_HUB_TOKEN` secret (see Setup step 2)

### Workflow Fails at "Commit and push manifest changes"

**Problem**: `Permission denied` or `refusing to allow a GitHub App to create or update workflow`

**Solution**: Enable write permissions (see Setup step 3)

### Images Build But Manifests Don't Update

**Problem**: Workflow succeeds but no commit is made

**Solution**: Check if the image tags in the manifests are already up to date. The workflow only commits if there are changes.

### ArgoCD Doesn't Pick Up Changes

**Problem**: Manifests updated but ArgoCD shows old version

**Solution**:
```bash
# Force ArgoCD to refresh from Git
argocd app get kbk-prod --refresh --hard-refresh

# Or sync manually
argocd app sync kbk-prod
```

## Monitoring

### View Workflow Runs
https://github.com/bubthegreat/kbk/actions

### View Docker Images
https://hub.docker.com/r/bubthegreat/kbk/tags

### Check Current Deployed Version

```bash
# What's in Git
cat k8s/base/kbk-deployment.yaml | grep image:

# What's running in Kubernetes
kubectl get deployment -n kbk-prod kbk-deployment -o jsonpath='{.spec.template.spec.containers[0].image}'
```

