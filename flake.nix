{
  description = "Timex Datalink Notebook Adapter emulator for Arduino";

  outputs = { self, nixpkgs }: let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in {
    devShell.x86_64-linux = pkgs.mkShell {
      buildInputs = [
        pkgs.micronucleus
        pkgs.arduino-cli
      ];
      shellHook = let
        arduino-cli = pkgs.lib.getExe' pkgs.arduino-cli "arduino-cli";
        micronucleus = pkgs.lib.getExe' pkgs.micronucleus "micronucleus";
      in ''
        ${arduino-cli} compile --profile digispark
        rm $HOME/.arduino15/internal/digistump_micronucleus_*_????????????????/micronucleus
        ln -s "${micronucleus}" $HOME/.arduino15/internal/digistump_micronucleus_*_????????????????
        ${arduino-cli} upload --profile digispark
      '';
    };
    
  };
}
