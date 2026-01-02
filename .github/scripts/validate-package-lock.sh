#!/bin/bash
set -e

# Script to validate package-lock.json uses only approved registry (Nexus only)
# Usage: validate-package-lock.sh <path-to-package-lock.json>

PACKAGE_LOCK_PATH="$1"

if [ -z "$PACKAGE_LOCK_PATH" ]; then
  echo "❌ Error: Missing package-lock.json path"
  echo "Usage: validate-package-lock.sh <path-to-package-lock.json>"
  exit 1
fi

if [ ! -f "$PACKAGE_LOCK_PATH" ]; then
  echo "❌ Error: File not found: $PACKAGE_LOCK_PATH"
  exit 1
fi

echo "🔍 Validating $PACKAGE_LOCK_PATH for approved registry URLs..."

# Check for public npm registry URLs
if grep -q "registry.npmjs.org" "$PACKAGE_LOCK_PATH"; then
  echo "❌ ERROR: package-lock.json contains registry.npmjs.org URLs. Please update package-lock dependencies with Nexus. The instruction can be found here https://www.notion.so/trustwallet/Nexus-NPM-Registry-Developer-Setup-Guide-2be904d52a648024b037f58be529def3?source=copy_link"
  echo ""
  echo "Found the following entries:"
  grep "registry.npmjs.org" "$PACKAGE_LOCK_PATH" | head -10
  echo ""
  echo "Please regenerate package-lock.json with Nexus registry:"
  echo "  cd $(dirname "$PACKAGE_LOCK_PATH")"
  echo "  rm package-lock.json"
  echo "  npm install"
  echo "  git add package-lock.json"
  echo "  git commit -m 'chore: update package-lock.json to use Nexus registry'"
  exit 1
fi

# Check for GitHub Packages URLs (no longer allowed)
if grep -q "npm.pkg.github.com" "$PACKAGE_LOCK_PATH"; then
  echo "❌ ERROR: package-lock.json contains npm.pkg.github.com URLs. GitHub Packages are no longer supported. Please update to use Nexus registry."
  echo ""
  echo "Found the following entries:"
  grep "npm.pkg.github.com" "$PACKAGE_LOCK_PATH" | head -10
  echo ""
  echo "Please regenerate package-lock.json with Nexus registry:"
  echo "  cd $(dirname "$PACKAGE_LOCK_PATH")"
  echo "  rm package-lock.json"
  echo "  npm install"
  echo "  git add package-lock.json"
  echo "  git commit -m 'chore: migrate from GitHub Packages to Nexus registry'"
  exit 1
fi

# Verify Nexus registry URLs are present
NEXUS_COUNT=$(grep -c "nexus.twinternal.net" "$PACKAGE_LOCK_PATH" || true)

if [ "$NEXUS_COUNT" -eq 0 ]; then
  echo "❌ ERROR: No Nexus URLs found in package-lock.json"
  echo "Expected to find URLs containing: nexus.twinternal.net"
  echo ""
  echo "Please ensure your .npmrc is configured correctly and regenerate package-lock.json"
  exit 1
fi

echo "✅ package-lock.json validation passed"
echo "   📦 Nexus URLs: $NEXUS_COUNT"
