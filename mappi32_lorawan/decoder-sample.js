function Decoder(bytes, port) {

  var suhu = ((bytes[0]) << 24)
              + ((bytes[1]) << 16)
              + ((bytes[2]) << 8)
              + ((bytes[3])); 
                    
  var kelembaban = ((bytes[4]) << 24)
              + ((bytes[5])<< 16)
              + ((bytes[6]) << 8)
              + ((bytes[7]));
              
  var uvA = ((bytes[8]) << 24)
              + ((bytes[9])<< 16)
              + ((bytes[10]) << 8)
              + ((bytes[11]));
             

  var uvB = ((bytes[12]) << 24)
              + ((bytes[13])<< 16)
              + ((bytes[14]) << 8)
              + ((bytes[15]));
              
  var uvIndex = ((bytes[16]) << 24)
              + ((bytes[17])<< 16)
              + ((bytes[18]) << 8)
              + ((bytes[19]));

  return {
      field1: suhu/100,
      field2: kelembaban/100,
      field3: uvA/100,
      field4: uvB/100,
      field5: uvIndex/100
  };
}
