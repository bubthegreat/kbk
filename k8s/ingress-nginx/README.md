# ingress-nginx TCP passthrough for telnet

Telnet traffic reaches the MUD like this:

```
player → 192.168.0.20:8989/8988/8987 (MetalLB LB IP on the ingress-nginx controller Service)
       → ingress-nginx controller (TCP proxy mode)
       → kbk-<env>/kbk-service:8989
```

## The trap: this repo only manages 1 of the 3 required pieces

TCP passthrough needs **three** things. Only the first lives in this repo:

1. **The `tcp-services` ConfigMap** (`tcp-services-configmap.yaml` here,
   synced by the `kbk-ingress-nginx-tcp-config` ArgoCD app).
2. **The controller flag** — the ingress-nginx controller container must be
   started with `--tcp-services-configmap=ingress-nginx/tcp-services`.
   Without it the ConfigMap is silently ignored.
3. **The Service ports** — the ingress-nginx controller's LoadBalancer
   Service must expose ports 8989/8988/8987, otherwise MetalLB never
   forwards them.

Pieces 2 and 3 were originally patched onto the controller by hand. Any
reinstall or helm upgrade of ingress-nginx (e.g. rebuilding the homelab)
reverts them — telnet dies while the ConfigMap in this repo still looks
correct. If direct TCP stops working after cluster maintenance, check
pieces 2 and 3 first:

```bash
# Piece 2: is the flag present?
kubectl get deploy -n ingress-nginx -o yaml | grep tcp-services-configmap

# Piece 3: are the ports on the LB service?
kubectl get svc -n ingress-nginx -o wide | grep -E '898[789]'
```

## Durable fix (helm-managed ingress-nginx)

Put the TCP map in the ingress-nginx **helm values** wherever the homelab
manages that release. The chart then wires the flag, its own ConfigMap,
and the Service ports automatically, and they survive every upgrade:

```yaml
tcp:
  "8989": "kbk-prod/kbk-service:8989"
  "8988": "kbk-staging/kbk-service:8989"
  "8987": "kbk-dev/kbk-service:8989"
```

Note: with helm-managed `tcp:` values, the chart creates its own ConfigMap
(`<release>-ingress-nginx-tcp`) and this repo's `tcp-services` ConfigMap +
ArgoCD app become redundant — retire them at that point so there is a
single source of truth.

## Quick fix (manual, will not survive the next helm upgrade)

```bash
# Add the flag (adjust deployment name to your release)
kubectl patch deploy -n ingress-nginx ingressnginx-ingress-nginx-controller \
  --type json -p '[{"op":"add","path":"/spec/template/spec/containers/0/args/-","value":"--tcp-services-configmap=ingress-nginx/tcp-services"}]'

# Expose the ports on the LB service
kubectl patch svc -n ingress-nginx ingressnginx-ingress-nginx-controller \
  --type json -p '[
    {"op":"add","path":"/spec/ports/-","value":{"name":"kbk-prod","port":8989,"protocol":"TCP","targetPort":8989}},
    {"op":"add","path":"/spec/ports/-","value":{"name":"kbk-staging","port":8988,"protocol":"TCP","targetPort":8988}},
    {"op":"add","path":"/spec/ports/-","value":{"name":"kbk-dev","port":8987,"protocol":"TCP","targetPort":8987}}
  ]'
```

## Verifying end to end

```bash
kubectl get configmap -n ingress-nginx tcp-services -o yaml   # piece 1
kubectl get deploy -n ingress-nginx -o yaml | grep tcp-services-configmap  # piece 2
kubectl get svc -n ingress-nginx | grep -E '898[789]'         # piece 3
telnet 192.168.0.20 8987                                      # dev, end to end
```
