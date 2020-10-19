#! /bin/bash
OUTFILE=$1

echo "Test cases..."
RETGLOBAL=$(echo 0)

echo "1. Printing help"
    $OUTFILE -h > /dev/null
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Printing help";RETGLOBAL=$(echo 1); else echo " OK"; fi

echo "2. Test normal execution"
    $OUTFILE -i inputs/input.txt -o inputs/outputout.txt
    diff inputs/outputout.txt tests/outputout.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test normal execution";RETGLOBAL=$(echo 1); else echo " OK"; fi

echo "3. Test decode"
    $OUTFILE -d -i inputs/qb64.txt -o inputs/qb64out.txt
    diff inputs/qb64out.txt tests/qb64out.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test decode";RETGLOBAL=$(echo 1); else echo " OK"; fi

echo "4. Test empty input"
    $OUTFILE -i inputs/empty.txt -o inputs/emptyout.txt
    diff inputs/emptyout.txt tests/emptyout.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test empty input";RETGLOBAL=$(echo 1); else echo " OK"; fi

echo "5. Test long input"
    $OUTFILE -i inputs/long.txt -o inputs/longout.txt
    diff inputs/longout.txt tests/longout.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test long input";RETGLOBAL=$(echo 1); else echo " OK"; fi

echo "6. Test multiple of three input"
    $OUTFILE -i inputs/multiplode3.txt -o inputs/multiplode3out.txt
    diff inputs/multiplode3out.txt tests/multiplode3out.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test multiple of three input";RETGLOBAL=$(echo 1); else echo " OK"; fi

echo "7. Test pipe input, stdout output"
    cat inputs/input.txt | $OUTFILE -o - > inputs/pipeinputout.txt
    diff inputs/pipeinputout.txt tests/pipeinputout.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test pipe input, stdout output";RETGLOBAL=$(echo 1); else echo " OK"; fi    

echo "8. Test redirect symbols input output"
    $OUTFILE < inputs/input.txt > inputs/pipeinout.txt
    diff inputs/pipeinout.txt tests/pipeinout.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test redirect symbols input output";RETGLOBAL=$(echo 1); else echo " OK"; fi  

echo "9. Test redirect symbols only input"
    $OUTFILE -o inputs/pipeonlyinputout.txt < inputs/input.txt
    diff inputs/pipeonlyinputout.txt tests/pipeonlyinputout.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test redirect symbols only input";RETGLOBAL=$(echo 1); else echo " OK"; fi  

echo "10. Test redirect symbols only output"
    $OUTFILE -i inputs/input.txt > inputs/pipeinputonlyout.txt
    diff inputs/pipeinputonlyout.txt tests/pipeinputonlyout.txt > /dev/null 2>&1 
    RETVAL=$(echo $?)
    if [ $RETVAL -ne 0 ]; then  echo "  Error on Test redirect symbols only output";RETGLOBAL=$(echo 1); else echo " OK"; fi 

if [ $RETGLOBAL -eq 0 ]; then echo "ALL OK"; else echo "FAIL!"; fi