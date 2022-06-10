jq -c '."editor.formatOnSave" = true' .vscode/settings.json > settings.tmp 
rm -f .vscode/settings.json
mv settings.tmp .vscode/settings.json
rm -f settings.tmp
echo "Autoformat enabled"
echo "Restart VS Code to apply changes"