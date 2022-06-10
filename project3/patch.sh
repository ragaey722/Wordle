echo "Enable unicode support"
sudo pacman -S --noconfirm noto-fonts-emoji
sudo curl https://pastebin.com/raw/qtuWahcs -o /etc/fonts/conf.d/75-noto-color-emoji.conf 
echo "Test Unicode (might need a terminal restart): 🟩 🟨 ⬛ 🟢 🟡 "
echo "Install packages for the GUI"
sudo pacman -S --noconfirm gtk4
echo "Initialize support for headerfiles in VSCode"
echo -e "CompileFlags:\n  Add:\n    - \"--include-directory=$(pwd)/include\"" > .clangd
echo "Install linters for style and code quality"
sudo pacman -S --noconfirm cppcheck python-cpplint
echo "Install auxiliary program"
sudo pacman -S --noconfirm jq
echo "Install extension to use cpplint and cppcheck in VSCode"
wget https://QiuMingGe.gallery.vsassets.io/_apis/public/gallery/publisher/QiuMingGe/extension/cpp-check-lint/1.4.7/assetbyname/Microsoft.VisualStudio.Services.VSIXPackage 
mv Microsoft.VisualStudio.Services.VSIXPackage cppcheck.vsix
code --install-extension cppcheck.vsix
rm -f cppcheck.vsix