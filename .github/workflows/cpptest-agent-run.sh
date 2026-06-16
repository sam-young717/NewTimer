set -euo pipefail

# This script should run AI Agent for C/C++test Static Analysis with AI Autofix.
# It is intended to be executed by 'cpptest-autofix-github.yml' but can also be run manually.

COPILOT_BIN="/home/syoung/.local/bin/copilot"
# Override at the job level by exporting CPPTEST_MCP_BIN before running this script.
CPPTEST_MCP_BIN="${CPPTEST_MCP_BIN:-/mnt/c/Users/syoung/Documents/Solutions/Linux/2026.1/Standard/cpptest/integration/mcp/cpptestmcp}"

# Sanity-check the MCP server binary up front so failures are visible in the job log
# (otherwise Copilot will just report "MCP tool unavailable" with no detail).
if [ ! -x "$CPPTEST_MCP_BIN" ]; then
  echo "ERROR: cpptest MCP binary not found or not executable at: $CPPTEST_MCP_BIN" >&2
  ls -l "$CPPTEST_MCP_BIN" >&2 || true
  exit 1
fi

# Re-register every run so the path can never drift from a stale config
"$COPILOT_BIN" mcp remove cpptest-std-mcp >/dev/null 2>&1 || true
"$COPILOT_BIN" mcp add cpptest-std-mcp -- "$CPPTEST_MCP_BIN"

echo "Registered MCP servers:"
"$COPILOT_BIN" mcp list || true

# Verify Copilot CLI has an authentication token. The CLI accepts any of
# COPILOT_GITHUB_TOKEN, GH_TOKEN, or GITHUB_TOKEN. The default GITHUB_TOKEN
# minted by Actions does NOT have Copilot scope, so prefer COPILOT_GITHUB_TOKEN
# (a Fine-Grained PAT for a user with a Copilot subscription) when running in CI.
if [ -z "${COPILOT_GITHUB_TOKEN:-}" ] && [ -z "${GH_TOKEN:-}" ] && [ -z "${GITHUB_TOKEN:-}" ]; then
  echo "ERROR: No Copilot CLI authentication token found." >&2
  echo "Set the COPILOT_GITHUB_TOKEN secret in the repository settings" >&2
  echo "(Settings -> Secrets and variables -> Actions) to a PAT for a user with Copilot access." >&2
  exit 1
fi

# Execute the prompt with Copilot.
# --allow-all-tools auto-approves every tool (including MCP tools) in headless mode,
# and the explicit --allow-tool='cpptest-std-mcp' is belt-and-suspenders for that server.
"$COPILOT_BIN" \
  --allow-all-tools \
  --allow-tool='cpptest-std-mcp' \
  -p "$(cat "$(dirname "$0")/cpptest-agent-prompt.md")"