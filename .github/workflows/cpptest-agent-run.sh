#!/bin/bash

set -euo pipefail

# == Copilot ==

# Path to the working GitHub Copilot CLI binary on this self-hosted runner.
# (Avoids the broken Node-8 npm shim that shadows `copilot` on PATH.)
COPILOT_BIN="$HOME/.local/bin/copilot"

# Register C/C++test MCP server - assuming C/C++test is installed in '/opt/Parasoft/cpptest' location, adjust as needed
"$COPILOT_BIN" mcp get cpptest-std-mcp >/dev/null 2>&1 || \
  "$COPILOT_BIN" mcp add cpptest-std-mcp -- /opt/Parasoft/cpptest/integration/mcp/cpptestmcp

# Execute the prompt with Copilot - be sure to adjust sandbox permissions as needed for your prompt
"$COPILOT_BIN" --allow-all --no-ask-user -s -p "$(cat "$(dirname "$0")/cpptest-agent-prompt.md")"