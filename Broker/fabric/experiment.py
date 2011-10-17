import network

class Experiment(object):
    def __init__(self,host2uuid,granularity):
        self.expcounter = [0] * ((len(host2uuid) * (len(host2uuid)-1))/2)
        self.host2uuid = host2uuid
        self.granularity = granularity
    def maptonetwork(self):
        out = dict()
        index = 0
        seen = []
        for (hostx,uuidx) in self.host2uuid.iteritems():
            out.setdefault(hostx,dict())
            for (hosty,uuidy) in self.host2uuid.iteritems():
                out.setdefault(hosty,dict())
                if uuidx == uuidy:
                    continue
                if uuidy in seen:
                    continue
                assert not hosty in out[hostx], 'Key %s already in %s' % (hosty,hostx)
                assert not hostx in out[hosty], 'Key %s already in %s' % (hostx,hosty)
                out[hostx].setdefault(hosty, self.expcounter[index])
                out[hosty].setdefault(hostx, self.expcounter[index])
                index += 1
                seen.append(uuidx)
        assert index == len(self.expcounter), 'Did not use all exp settings'
        return out
    def next(self,default=None):
        index = 0
        if self.expcounter == [100]*(len(self.expcounter)):
            return default
        while index < len(self.expcounter):
            if self.expcounter[index] == 100:
                self.expcounter[index] = 0
                index += 1
            elif self.expcounter[index] < 100:
                self.expcounter[index] += self.granularity
                if self.expcounter[index] > 100:
                    self.expcounter[index] = 100
                break
        for x in self.expcounter:
            assert x <= 100, "Experiment has overrun"
        return self.expcounter
    def __repr__(self):
        return str(self.expcounter)
    def tsv_head(self):
        seen = []
        out = []
        for (hostx,uuidx) in self.host2uuid.iteritems():
            for (hosty,uuidy) in self.host2uuid.iteritems():
                if uuidy in seen:
                    continue
                if len(out) == 0:
                    tmp = "#"
                else:
                    tmp = ""
                tmp +="%sx%s" % (hostx,hosty)
                out.append(tmp)
        return "\t".join(out)
    def tsv_entry(self):
        return "\t".join([str(x) for x in self.expcounter])
    def generate_files(self):
        filedict = dict()
        m = self.maptonetwork()
        for (k,v) in m.iteritems():
            d = dict()
            for(k2,v2) in v.iteritems():
                # make a new dict, translating hostnames to uuids
                d.setdefault(self.host2uuid[k2],v2)
            filedict.setdefault(k,network.create_network_file(100,d))
        return filedict    

if __name__ == "__main__":
    import random
    dictthing = {'google.com':'goog','amazon.com':'amaz','movingpictures.com':'mvpc','rush.com':'rush'}
    x = Experiment(dictthing,20)
    i = 0
    if 0:
        while x.next() != None:
            print x
            i += 1
        print i
        raw_input("Press Enter to continue...")
        x = Experiment(dictthing,7)
        i = 0
        while x.next() != None:
            print x
            i += 1
        print i
        raw_input("Press Enter to continue...")
    while 1:
        x = Experiment(dictthing,20)
        for i in xrange(random.randint(1,40000)):
            x.next()
        print x.generate_files()
        m = x.maptonetwork()
        for (k,v) in m.iteritems():
            for (k2,v2) in v.iteritems():
                if m[k][k2] != m[k2][k]:
                    print "BAD."
                    exit(1)
