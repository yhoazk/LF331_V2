# Monitor writting to a FS

## Option 1

```
sudo auditctl -w /tmp/my_tmp/ -p wa -k MY_FS
```