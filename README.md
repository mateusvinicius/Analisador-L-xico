# Analisador Léxico

Aluno:Mateus Vinicius Luz Da Silva 

Matricula : 152030413

Lista de símbolos:
"for step sqrt if else return print read to num string " ( ) { } = > < + - * / %"

Reconhecer como válido ou inválido os arquivos fonte como válido:

Codigo Valido :
```
num: val, i, resultado

resultado = 0

print "Digite um número: "
read val

for i = 2 to (val / 2) step 1
{
   if val % i = 0 {
      resultado = resultado + 1
      break
   }
}

if resultado = 0
{
   print "O" + val + " é um número primo"
}
else
{
   print "O" + val + " NÃO é um número primo"
}
```
Codigo Invalido :
```
num: val, i, esultado

resultado = 0

print "Digite um número: 
read val

for i = 2 to (val / 2) step 1
{
   if val % i = 0 {
      resultado = resultado + 1
      break
   
}

if resultado = 0
{
   print "O" + val + " é um número primo"
}
else
{
   print "O" + val + " NÃO é um número primo"
}

```


Saida do codigo valido :
```
-Analise léxica valida ! 
-Analise sintatica valida !
```
Saida do codigo invalido :
```
-Error Sintatico-linha:3: A palavra resultado nÃ£o Ã© uma palavra reservada e nem uma variavel.
-Error Sintatico-linha:5: A palavra O nÃ£o Ã© uma palavra reservada e nem uma variavel. 
-Error Sintatico-linha:5: A palavra um nÃ£o Ã© uma palavra reservada e nem uma variavel.
-Error Sintatico-linha:5: A palavra n nÃ£o Ã© uma palavra reservada e nem uma variavel. 
-Error Sintatico-linha:5: A palavra mero nÃ£o Ã© uma palavra reservada e nem uma variavel.
-Error Sintatico-linha:5: A palavra primo nÃ£o Ã© uma palavra reservada e nem uma variavel. 
-Error Sintatico-linha:5: A palavra O nÃ£o Ã© uma palavra reservada e nem uma variavel.
-Error Sintatico-linha:5: A palavra N nÃ£o Ã© uma palavra reservada e nem uma variavel. 
-Error Sintatico-linha:5: A palavra O nÃ£o Ã© uma palavra reservada e nem uma variavel.
-Error Sintatico-linha:5: A palavra um nÃ£o Ã© uma palavra reservada e nem uma variavel. 
-Error Sintatico-linha:5: A palavra n nÃ£o Ã© uma palavra reservada e nem uma variavel.
-Error Sintatico-linha:5: A palavra mero nÃ£o Ã© uma palavra reservada e nem uma variavel. 
-Error Sintatico-linha:5: A palavra primo nÃ£o Ã© uma palavra reservada e nem uma variavel.
```

