<?php
include("simple_html_dom.php");;

$html = file_get_html('https://www.marinha.mil.br/chm/dados-do-smm/iacfleet');
$ret = $html->find('div[class*=field-item"]', 0);
$str = $ret->find('p', 0)->plaintext;
preg_replace('/[[:^print:]]/', '', $str);
echo $str;
?>
