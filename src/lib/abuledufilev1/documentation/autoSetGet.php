<?php
//utilitaire qui fabrique tout seul le code cpp de getter et setter quand on
//a des tonnes de variables (ex. pour implemnter la norme LOM)

$fichier   = "lomv1_0.h";
$classname = "lomv1_0";
$prefix    = "LOM";

//ouverture du fichier
$contenu = file($fichier);
$codecpp = "";
$codeh   = "";
for($i = 0; $i < count($contenu); $i++) {
    $l = preg_replace('/\s\s+/',' ',trim($contenu[$i]));
    $ligne = explode(" ",$l);
    //    print_r($ligne);

    //Les QString
    if($ligne[0] == "QString") {
      $variable = str_replace(";","",$ligne[1]);
      $varSansM = str_replace("m_","",$variable);
      $codeh .= "void    " . $prefix . "set" . $varSansM . "(QString value);\n";
      $codeh .= "QString " . $prefix . "get" . $varSansM . "();\n\n";
      $codecpp .= "void $classname::". $prefix  ."set" . $varSansM . "(QString value)
{
  " . $variable . "=value;
}
QString $classname::" . $prefix . "get" . $varSansM . "()
{
  return " . $variable . ";
}

";
    }
    //Les paires lang,valeur
    if($ligne[0] == "LomLS") {
      $variable = str_replace(";","",$ligne[1]);
      $varSansM = str_replace("m_","",$variable);
      $codeh .= "void    " . $prefix . "set" . $varSansM . "(QString lang, QString value);\n";
      $codeh .= "QString " . $prefix . "get" . $varSansM . "(QString lang);\n";
      $codeh .= "QList<QString> " . $prefix . "get" . $varSansM . "Keys();\n\n";
      $codecpp .= "void $classname::" . $prefix . "set" . $varSansM . "(QString lang, QString value)
{
  " . $variable . ".set(lang,value);
}
QString $classname::" . $prefix . "get" . $varSansM . "(QString lang)
{
  return " . $variable . ".get(lang);
}
QList<QString> $classname::" . $prefix . "get" . $varSansM . "Keys()
{
  return " . $variable . ".keys();
}

";
    }

}
  
$fh = fopen("autosetget.h", "w");
if($fh==false)
  die("unable to create file");
fputs ($fh, $codeh);
fclose ($fh);

$fh = fopen("autosetget.cpp", "w");
if($fh==false)
  die("unable to create file");
fputs ($fh, $codecpp);
fclose ($fh);


print "\nCreation automatique des set/get teminee\noutput to autosetget.h and autosetget.cpp\n\n";
?>
