#!/usr/bin/env bash
# The eBPF loader attaches to a kernel tracepoint, which is exposed through
# tracefs. Containers do not mount tracefs by default, so mount it here (this
# requires --privileged). Failure is ignored so non-test uses of the image
# still work without extra privileges.
if [ ! -e /sys/kernel/tracing/events ]; then
  mount -t tracefs nodev /sys/kernel/tracing 2>/dev/null || true
fi

exec "$@"
