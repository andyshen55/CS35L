#! /bin/bash

#Matches lines enclosed in <td> tags
grep -E "<td>.*</td>" $@ |

#Removes tags and ? char
sed 's/?//g' |
sed 's/<u>//g' |
sed 's/<\/u>//g' | 
sed 's/<td>//g' |
sed "s/<\/td>//g" |

#Replaces ASCII grave accents with apostrophes
sed "s/\`/\'/g" |

#separates phrases into individual words (on separate lines)
tr ',' '\n' |
tr ' ' '\n' |

#truncates extraneous newlines
tr -s '\n' |

#converts to lowercase
tr [:upper:] [:lower:] |

#deletes lines containing non-Hawaiian alphabetical characters
sed "/[^pk\'mnwlhaeiou]/d" |

#filters unique words alphabetically
sort -u
